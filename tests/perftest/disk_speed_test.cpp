#include "disk_speed_test.h"
#include <iosfwd>

profiler_timer::profiler_timer(const char* _name)
	: name(_name)
{
	start_time = std::chrono::high_resolution_clock::now();
}
profiler_timer::~profiler_timer()
{
	auto									 end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::nano> delta = end - start_time;
	double									 ddelta = delta.count();

	std::cout << "--" << name << ": " << uint64_t(ddelta) << " ns";

	if (ddelta > 1000000.0)
	{
		// ms
		ddelta = ddelta / 1000000.0;

		if (ddelta > 1000)
		{
			// sec
			ddelta = ddelta / 1000.0;
			std::cout << " -> " << ddelta << " sec";
		}
		else
		{
			std::cout << " -> " << ddelta << " ms";
		}
	}

	std::cout << std::endl;
}

bool read_validator::validate(const uint32_t value, const char* reader)
{
	if (value != refvalue)
	{
		if (initialized == false)
		{
			refvalue = value;
			initialized = true;
		}
		else
		{
			//
			std::cerr << "Invalid test result (" << value << ") for reader " << reader << std::endl;
			return false;
		}
	}
	return true;
}

std::size_t disk_access_test::generate_sample_file(const char* path)
{
	const std::size_t size_in_bytes = std::size_t(1024) * 1024 * 16; // 64 MB
	const std::size_t num_of_ints = size_in_bytes / sizeof(uint32_t);

	{
		cllio::std_file_write f;

		if (f.create(path, true, false) == false)
			return 0; // failed to write file

		uint32_t index = 0;
		for (std::size_t i = 0; i < num_of_ints; i++)
		{
			if (f.trypush_uint32_t(index) == false)
			{
				std::cerr << "Failed to write to " << path << std::endl;
				return 0;
			}
			index++;
		}
		f.flush();
	}

	return size_in_bytes;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

struct virtual_memory_reader
{
public:
	cllio::file_read_mapview handle;
	const uint32_t*			 udata;

public:
	uint32_t open(const char* abs_file_path)
	{
		cllio::file_read_mapview tmp(abs_file_path);
		if (tmp.data() == nullptr)
			return 0;
		handle.swap(tmp);
		udata = static_cast<const uint32_t*>(handle.data());
		return handle.size() / sizeof(uint32_t);
	}
	inline std::size_t uint32_t_count() const
	{
		return handle.size() / sizeof(uint32_t);
	}
	inline uint32_t read(const std::size_t index) const
	{
		return udata[index];
	}
	const char* get_name() const
	{
		return "virtual_memory_reader";
	}
};

struct std_fread_simple
{
public:
	cllio::std_file_read handle;
	std::size_t			 size;

public:
	uint32_t open(const char* abs_file_path)
	{
		if (handle.open(abs_file_path, true) == false)
			return 0;
		size = handle.get_file_size() / sizeof(uint32_t);
		return size;
	}
	inline std::size_t uint32_t_count() const
	{
		return size;
	}
	inline uint32_t read(const std::size_t)
	{
		return handle.pop_uint32_t();
	}
	const char* get_name() const
	{
		return "std_fread_simple";
	}
};

struct std_fseek_fread : public std_fread_simple
{
public:
	inline uint32_t read(const std::size_t index)
	{
		std::fseek(handle.get_handle(), long(index * sizeof(uint32_t)), SEEK_SET);
		return handle.pop_uint32_t();
	}
	const char* get_name() const
	{
		return "std_fseek_fread";
	}
};

struct file_buffer_reader
{
public:
	std::vector<uint32_t> data;

public:
	uint32_t open(const char* abs_file_path)
	{
		cllio::std_file_read handle;
		if (handle.open(abs_file_path, true) == false)
			return 0;
		if (handle.read_into_container(data) == false)
			data.pop_back(); // last element was not complete
		return data.size();
	}

	inline std::size_t uint32_t_count() const
	{
		return data.size();
	}
	inline uint32_t read(const std::size_t index)
	{
		return data[index];
	}
	const char* get_name() const
	{
		return "file_buffer_reader";
	}
};

template <std::size_t SZ>
struct buffered_file_reader : public std_fread_simple
{
public:
	uint32_t	buffer[SZ];
	std::size_t start = std::numeric_limits<std::size_t>::max();
	std::size_t end = 0;

public:
	inline uint32_t read(const std::size_t index)
	{
		if (index >= start && index < end)
			return buffer[index - start];

		std::fseek(handle.get_handle(), long(index * sizeof(uint32_t)), SEEK_SET);
		start = index;
		end = std::min(index + SZ, uint32_t_count());
		if (std::fread(buffer, sizeof(uint32_t), end - start, handle.get_handle()) == 0)
			return 0; // err?
		return buffer[index - start];
	}
	const char* get_name() const
	{
		return "buffered_file_reader";
	}
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
bool test_seq_reader(read_validator& validator, T& reader)
{
	uint32_t result = 1;

	std::size_t size = reader.uint32_t_count();
	for (std::size_t i = 0; i < size; i++)
	{
		result = result ^ reader.read(i);
	}

	return validator.validate(result, reader.get_name());
}

template <class T>
bool test_rnd_reader(read_validator& validator, T& reader)
{
	uint32_t result = 1;

	std::size_t index = 1;
	std::size_t size = reader.uint32_t_count();

	for (std::size_t i = 0; i < size; i++)
	{
		index = index * 1104737;
		result = result ^ reader.read(index % size);
	}

	return validator.validate(result, reader.get_name());
}

template <class T>
bool test_rnd_chunk_reader(read_validator& validator, T& reader, const std::size_t chunk_size, const std::size_t number_of_reads)
{
	uint32_t result = 1;

	std::size_t index = 1;
	std::size_t size = reader.uint32_t_count();

	for (std::size_t i = 0; i < number_of_reads; i++)
	{
		index = index * 1106531;
		std::size_t _start = index % size;
		std::size_t _end = std::min(_start + chunk_size, size);
		for (std::size_t j = _start; j < _end; j++)
			result = result ^ reader.read(j);
	}

	return validator.validate(result, reader.get_name());
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

bool disk_access_test::run(const char* path_to_test_file)
{
	const char* final_file = path_to_test_file;
	uint32_t	sz = 0;
	bool		known_content = false;
	if (final_file == nullptr)
	{
		final_file = "iosample.bin";
		sz = generate_sample_file(final_file);
		if (sz == 0)
		{
			std::cerr << "Failed to generate file in current working directory.\n";
			return false;
		}
		known_content = true;
	}

	std::cout << "----------- INFO -----------\n";

	std::cout << "using " << final_file << std::endl;
	if (sz != 0)
	{
		std::cout << "generated content of " << sz << " bytes\n";
	}
	else
	{
		cllio::std_file_read handle;
		if (handle.open(path_to_test_file))
		{
			sz = handle.get_file_size();
			std::cout << "size " << sz << " bytes\n";
		}
		else
		{
			std::cerr << "Error, failed to open file.\n";
			return false;
		}
	}
	sz = sz / sizeof(uint32_t);

	return run_internal(final_file, known_content, sz);
}

bool disk_access_test::run_internal(const char* path_to_test_file, const bool known_content, const std::size_t known_content_size)
{
	read_validator size_validator;
	read_validator seq_validator;
	read_validator rnd_validator;
	read_validator rnd_chunk_validator;

	size_validator.validate(known_content_size, "init");
	if (known_content)
	{
		seq_validator.validate(uint32_t(1), "init seq");
		rnd_validator.validate(uint32_t(1), "init rnd");
		rnd_chunk_validator.validate(uint32_t(65537), "init rnd chunk");
	}

	std::cout << "----------- RUNNING SEQ READ -----------\n";
	{
		// test uint32_t seq read
		{
			virtual_memory_reader r;
			{
				profiler_timer _("mmap [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("mmap data iterate [seq]");
				if (test_seq_reader(seq_validator, r) == false)
					return false;
			}
		}
		{
			std_fread_simple r;
			{
				profiler_timer _("fopen/get file size [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("fread [seq]");
				if (test_seq_reader(seq_validator, r) == false)
					return false;
			}
		}
		{
			std_fseek_fread r;
			{
				profiler_timer _("fopen/get file size [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("fseek + freed [seq]");
				if (test_seq_reader(seq_validator, r) == false)
					return false;
			}
		}
		{
			file_buffer_reader r;
			{
				profiler_timer _("fopen/read entire file to std::vector [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("iterate std::vector [seq]");
				if (test_seq_reader(seq_validator, r) == false)
					return false;
			}
		}
		{
			buffered_file_reader<256> r;
			{
				profiler_timer _("fopen/get file size [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("fseek + fread buffered(std::vector) [seq]");
				if (test_seq_reader(seq_validator, r) == false)
					return false;
			}
		}
	}

	std::cout << "----------- RUNNING RND READ -----------\n";

	{
		// test uint32_t rnd read
		{
			virtual_memory_reader r;
			{
				profiler_timer _("mmap [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("mmap data iterate [rnd]");
				if (test_rnd_reader(rnd_validator, r) == false)
					return false;
			}
		}
		{
			std_fseek_fread r;
			{
				profiler_timer _("fopen/get file size [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("fseek + fread [rnd]");
				if (test_rnd_reader(rnd_validator, r) == false)
					return false;
			}
		}
		{
			file_buffer_reader r;
			{
				profiler_timer _("fopen/read entire file to std::vector [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("iterate std::vector [rnd]");
				if (test_rnd_reader(rnd_validator, r) == false)
					return false;
			}
		}
		{
			buffered_file_reader<256> r;
			{
				profiler_timer _("fopen/get file size [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("fseek + fread buffered(std::vector) [rnd]");
				if (test_rnd_reader(rnd_validator, r) == false)
					return false;
			}
		}
	}

	std::size_t number_of_reads = 4096;
	std::size_t chunk_size = 64;
	std::cout << "----------- RUNNING RND CHUNK READ -----------\n";
	std::cout << number_of_reads << " reads\n";
	std::cout << chunk_size * sizeof(uint32_t) << " bytes chunk\n";

	{
		// test uint32_t rnd read
		{
			virtual_memory_reader r;
			{
				profiler_timer _("mmap [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("mmap data iterate [rnd chunk]");
				if (test_rnd_chunk_reader(rnd_chunk_validator, r, chunk_size, number_of_reads) == false)
					return false;
			}
		}
		{
			std_fseek_fread r;
			{
				profiler_timer _("fopen/get file size [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("fseek + fread [rnd chunk]");
				if (test_rnd_chunk_reader(rnd_chunk_validator, r, chunk_size, number_of_reads) == false)
					return false;
			}
		}
		{
			file_buffer_reader r;
			{
				profiler_timer _("fopen/read entire file to std::vector [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("iterate std::vector [rnd chunk]");
				if (test_rnd_chunk_reader(rnd_chunk_validator, r, chunk_size, number_of_reads) == false)
					return false;
			}
		}
		{
			buffered_file_reader<256> r;
			{
				profiler_timer _("fopen/get file size [open]");
				if (size_validator.validate(r.open(path_to_test_file), r.get_name()) == false)
					return false;
			}
			{
				profiler_timer _("fseek + fread buffered(std::vector) [rnd chunk]");
				if (test_rnd_chunk_reader(rnd_chunk_validator, r, chunk_size, number_of_reads) == false)
					return false;
			}
		}
	}

	std::cout << "----------- END -----------\n";

	return true;
}