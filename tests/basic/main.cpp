
#include <cllio.h>
#include <limits>
#include <iostream>
#include <vector>
#include <cstring>
#include <functional>
#include <cllio_tools/write_size.h>
#include <cllio_tools/file_read_mapview.h>

const auto	ref_file_path = "../../../tests/samples.bin";
const auto	file_path = "samples.bin";
std::size_t failed = 0;

void on_func_failed()
{
	failed++;
	std::cerr << "FAILED\n" << std::endl;
}

template <class T>
T not_value(const T& value)
{
	union
	{
		uint8_t data[sizeof(T)];
		T		value;
	} buffer;
	buffer.value = value;
	for (std::size_t i = 0; i < sizeof(T); i++)
		buffer.data[i] = ~buffer.data[i];
	return buffer.value;
}

void check_buffers_equal(const std::vector<cllio::byte_t>& a, const std::vector<cllio::byte_t>& b, const std::size_t sz)
{
	TEST_ASSERT(a.size() >= sz);
	TEST_ASSERT(b.size() >= sz);
	if (std::memcmp(a.data(), b.data(), b.size()) != 0)
	{
		for (std::size_t i = 0; i < sz; i++)
		{
			TEST_ASSERT(a[i] == b[i]);
		}
	}
}

template <class T>
void test_writer(T& stream)
{
#define TEST_ITEM(TYPE, VALUE) stream.push_##TYPE(VALUE)

#include "test_set.h"

#undef TEST_ITEM
}

template <class T>
void run_read_functions_f0(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)        \
	{                                 \
		TYPE v = stream.pop_##TYPE(); \
		TEST_ASSERT(VALUE == v);      \
	}

#include "test_set.h"

#undef TEST_ITEM
}

template <class T>
void run_read_functions_f1(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                    \
	{                                             \
		TYPE value = not_value(VALUE);            \
		bool f = stream.pop_##TYPE(value);        \
		TEST_ASSERT(VALUE == value && f == true); \
	}

#include "test_set.h"

#undef TEST_ITEM
}
template <class T>
void run_read_functions_f2(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                     \
	{                                              \
		bool r = false;                            \
		TYPE value = stream.pop_##TYPE(r);         \
		TEST_ASSERT(VALUE == value && r == false); \
	}

#include "test_set.h"

#undef TEST_ITEM
}

template <class T>
void run_read_functions_f3(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                                      \
	{                                                               \
		TYPE value = not_value(VALUE);                              \
		bool f = stream.popdefault_##TYPE(value, not_value(VALUE)); \
		TEST_ASSERT(VALUE == value && f == true);                   \
	}

#include "test_set.h"

#undef TEST_ITEM
}

template <class T>
void run_read_functions_f4(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                                   \
	{                                                            \
		TYPE value = stream.popdefault_##TYPE(not_value(VALUE)); \
		TEST_ASSERT(VALUE == value);                             \
	}

#include "test_set.h"

#undef TEST_ITEM
}

void test_file_reader()
{
	{
		cllio::stdfile_rstream in;
		TEST_ASSERT(in.open(ref_file_path, true));
		run_read_functions_f0(in);
	}
	{
		cllio::stdfile_rstream in;
		TEST_ASSERT(in.open(ref_file_path, true));
		run_read_functions_f1(in);
	}
	{
		cllio::stdfile_rstream in;
		TEST_ASSERT(in.open(ref_file_path, true));
		run_read_functions_f2(in);
	}
	{
		cllio::stdfile_rstream in;
		TEST_ASSERT(in.open(ref_file_path, true));
		run_read_functions_f3(in);
	}
	{
		cllio::stdfile_rstream in;
		TEST_ASSERT(in.open(ref_file_path, true));
		run_read_functions_f4(in);
	}
}
void test_memory_readers(const std::vector<uint8_t>& buffer)
{
	{
		cllio::memory_rstream_unchecked ms(buffer.data());
		run_read_functions_f0(ms);
	}

	{
		cllio::memory_rstream ms(buffer.data(), buffer.size());
		run_read_functions_f0(ms);
	}
	{
		cllio::memory_rstream ms(buffer.data(), buffer.size());
		run_read_functions_f1(ms);
	}
	{
		cllio::memory_rstream ms(buffer.data(), buffer.size());
		run_read_functions_f2(ms);
	}
	{
		cllio::memory_rstream ms(buffer.data(), buffer.size());
		run_read_functions_f3(ms);
	}
	{
		cllio::memory_rstream ms(buffer.data(), buffer.size());
		run_read_functions_f4(ms);
	}
}

void run_main_tests()
{
	const std::size_t sample_file_expected_size = 1134;
	{
		// write sample file
		cllio::stdfile_wstream out;
		TEST_ASSERT(out.create(file_path, true, false));
		test_writer(out);
	}

	{
		// get output size
		cllio::write_size f;
		test_writer(f);
		TEST_ASSERT(f.size() == sample_file_expected_size);
		/*{
			std::cerr << "Error: cllio::size_info size mismatch.\n";
			return false;
		}*/
	}

	TEST_FUNCTION(test_file_reader);

	{
		// memory test
		std::vector<cllio::byte_t> buffer;

		{
			cllio::stdfile_rstream in;
			TEST_ASSERT(in.open(ref_file_path, true));
			TEST_ASSERT(in.isOpen());
			/*{
				std::cerr << "Failed open test file.\n";
				return false;
			}*/
			TEST_ASSERT(in.get_file_size() == sample_file_expected_size);
			/*{
				std::cerr << "Error: Test file Size mismatch.\n";
				return false;
			}*/
			in.read_into_container(buffer);
			TEST_ASSERT(buffer.size() == sample_file_expected_size);
			/*{
				std::cerr << "Failed to read entire file into container.\n";
				return false;
			}*/
			in.close();
			TEST_ASSERT(in.isOpen() == false);
			/*{
				std::cerr << "Error: Input file should be closed.\n";
				return false;
			}*/
		}

		{
			cllio::file_read_mapview in(ref_file_path);
			TEST_ASSERT(in.size() == buffer.size());
			/*{
				std::cerr << "Error: cllio::file_read_mapview size mismatch.\n";
				return false;
			}*/
			bool data_ok = std::memcmp(in.data(), in.data(), buffer.size()) == 0;
			TEST_ASSERT(data_ok);
			/*{
				std::cerr << "Error: cllio::file_read_mapview data mismatch.\n";
				return false;
			}*/
		}

		TEST_INLINE() = [&]() { test_memory_readers(buffer); };

		{
			// since there is no normal way to check memory_wstream_unchecked UB we reserve a double sized buffer for this
			// we then run the write operations and see if they match
			std::vector<cllio::byte_t> test_buffer;
			test_buffer.resize(buffer.size() * 2);
			std::memcpy(test_buffer.data(), buffer.data(), buffer.size());

			cllio::memory_wstream_unchecked writer(test_buffer.data());
			test_writer(writer);

			check_buffers_equal(test_buffer, buffer, buffer.size());
			bool iterator_ok = writer.data() == (test_buffer.data() + buffer.size());
			TEST_ASSERT(iterator_ok);
			/*{
				std::cerr << "Error: cllio::memory_wstream_unchecked data or iterator mismatch.\n";
				return false;
			}*/
		}
		{
			std::vector<cllio::byte_t> test_buffer;
			test_buffer.resize(buffer.size());
			cllio::memory_wstream writer(test_buffer.data(), test_buffer.size());
			test_writer(writer);

			check_buffers_equal(test_buffer, buffer, buffer.size());
			bool iterator_ok = writer.begin() == (test_buffer.data() + buffer.size());
			TEST_ASSERT(iterator_ok);
			/*{
				std::cerr << "Error: cllio::memory_wstream data or iterator mismatch.\n";
				return false;
			}*/
		}
		{
			std::vector<cllio::byte_t> test_buffer;
			auto					   functor_writer = cllio::memory_wfunc<std::function<cllio::byte_t*(const std::size_t)>> { [&](const std::size_t ns) {
				auto				   sz = test_buffer.size();
				test_buffer.resize(sz + ns);
				return test_buffer.data() + sz;
			} };

			test_writer(functor_writer);
			bool data_ok = std::memcmp(test_buffer.data(), buffer.data(), buffer.size()) == 0;
			TEST_ASSERT(data_ok);
			/*{
				std::cerr << "Error: cllio::memory_functor_write data or iterator mismatch.\n";
				return false;
			}*/
		}
	}
}

void test_utils()
{
	auto check_value = [](const uint64_t i) {
		char buffer[16];
		std::memset(&buffer[0], 0, 16);

		uint64_t			  ref = i;
		cllio::memory_wstream w(&buffer[4], 8);
		if (cllio::utils::write_packed_uint64_t(w, ref) == false)
			return false;

		uint64_t			  check = std::numeric_limits<uint64_t>::max();
		cllio::memory_rstream r(&buffer[4], 8);
		if (cllio::utils::read_packed_uint64_t(r, check) == false)
			return false;

		if (check != ref)
			return false;

		char buffer_check = buffer[0] | buffer[1] | buffer[2] | buffer[3];
		buffer_check = buffer[12] | buffer[13] | buffer[14] | buffer[15];
		if (buffer_check != 0)
			return false;

		return true;
	};
	auto check_and_print = [&](const uint64_t& value) {
		auto r = check_value(value);
		if (r == false)
			std::cerr << "packed uint64 read/write failed with value " << value << std::endl;
		return r;
	};
	for (uint64_t i = 0; i < 2048; i++)
	{
		check_and_print(i);
		check_and_print(i + std::numeric_limits<uint16_t>::max() - 1024);
		check_and_print(i + std::numeric_limits<uint32_t>::max() - 1024);
		check_and_print(i * 104729);
		check_and_print((i + std::numeric_limits<uint16_t>::max() - 1024) * 104729);
		check_and_print((i + std::numeric_limits<uint32_t>::max() - 1024) * 104729);
	}
}

void test_main()
{
	TEST_FUNCTION(run_main_tests);
	TEST_FUNCTION(test_utils);
}
TEST_MAIN(test_main)
