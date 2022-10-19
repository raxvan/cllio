
#include <cllio.h>
#include <limits>
#include <iostream>
#include <vector>
#include <cstring>
#include <functional>

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
#define TEST_ITEM(TYPE, VALUE)                                                              \
	{                                                                                       \
		TYPE v = stream.pop_##TYPE();                                                       \
		if ((VALUE) != v)                                                                   \
		{                                                                                   \
			on_func_failed();                                                               \
			std::cerr << "FAILED " #TYPE "pop_" #TYPE "() with value " #VALUE << std::endl; \
		}                                                                                   \
		else                                                                                \
			std::cout << "OK " #TYPE " pop_" #TYPE "() with value " << #VALUE << std::endl; \
	}

#include "test_set.h"

#undef TEST_ITEM
}

template <class T>
void run_read_functions_f1(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                                                                    \
	{                                                                                             \
		TYPE value = not_value(VALUE);                                                            \
		bool f = stream.pop_##TYPE(value);                                                        \
		if ((VALUE) != value || f == false)                                                       \
		{                                                                                         \
			on_func_failed();                                                                     \
			std::cerr << "FAILED bool pop_" #TYPE "(" #TYPE "&) with value " #VALUE << std::endl; \
		}                                                                                         \
		else                                                                                      \
			std::cout << "OK bool pop_" #TYPE "(" #TYPE "&) with value " << #VALUE << std::endl;  \
	}

#include "test_set.h"

#undef TEST_ITEM
}
template <class T>
void run_read_functions_f2(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                                                                    \
	{                                                                                             \
		bool r = false;                                                                           \
		TYPE value = stream.pop_##TYPE(r);                                                        \
		if ((VALUE) != value || r == true)                                                        \
		{                                                                                         \
			on_func_failed();                                                                     \
			std::cerr << "FAILED " #TYPE " pop_" #TYPE "(bool&) with value " #VALUE << std::endl; \
		}                                                                                         \
		else                                                                                      \
			std::cout << "OK " #TYPE " pop_" #TYPE "(bool&) with value " #VALUE << std::endl;     \
	}

#include "test_set.h"

#undef TEST_ITEM
}

template <class T>
void run_read_functions_f3(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                                                                                             \
	{                                                                                                                      \
		TYPE value = not_value(VALUE);                                                                                     \
		bool f = stream.popdefault_##TYPE(value, not_value(VALUE));                                                        \
		if ((VALUE) != value || f == false)                                                                                \
		{                                                                                                                  \
			on_func_failed();                                                                                              \
			std::cerr << "FAILED bool popdefault_" #TYPE "(" #TYPE "&, const " #TYPE "&) with value " #VALUE << std::endl; \
		}                                                                                                                  \
		else                                                                                                               \
			std::cout << "OK bool popdefault_" #TYPE "(" #TYPE "&, const " #TYPE "&) with value " << #VALUE << std::endl;  \
	}

#include "test_set.h"

#undef TEST_ITEM
}

template <class T>
void run_read_functions_f4(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                                                                                      \
	{                                                                                                               \
		TYPE value = stream.popdefault_##TYPE(not_value(VALUE));                                                    \
		if ((VALUE) != value)                                                                                       \
		{                                                                                                           \
			on_func_failed();                                                                                       \
			std::cerr << "FAILED " #TYPE " popdefault_" #TYPE "(const " #TYPE "&) with value " #VALUE << std::endl; \
		}                                                                                                           \
		else                                                                                                        \
			std::cout << "OK " #TYPE " popdefault_" #TYPE "(const " #TYPE "&) with value " << #VALUE << std::endl;  \
	}

#include "test_set.h"

#undef TEST_ITEM
}

void TestFileReaders()
{
	TEST_INLINE() = [&]() {
		cllio::std_file_read in;
		in.open(ref_file_path, true);
		run_read_functions_f0(in);
	};

	TEST_INLINE() = [&]() {
		cllio::std_file_read in;
		in.open(ref_file_path, true);
		run_read_functions_f1(in);
	};

	TEST_INLINE() = [&]() {
		cllio::std_file_read in;
		in.open(ref_file_path, true);
		run_read_functions_f2(in);
	};

	TEST_INLINE() = [&]() {
		cllio::std_file_read in;
		in.open(ref_file_path, true);
		run_read_functions_f3(in);
	};

	TEST_INLINE() = [&]() {
		cllio::std_file_read in;
		in.open(ref_file_path, true);
		run_read_functions_f4(in);
	};
}
void TestMemoryReaders(const std::vector<uint8_t>& buffer)
{
	TEST_INLINE() = [&]() {
		cllio::mem_stream_read_unchecked ms(buffer.data());
		run_read_functions_f0(ms);
	};

	TEST_INLINE() = [&]() {
		cllio::mem_stream_read ms(buffer.data(), buffer.size());
		run_read_functions_f0(ms);
	};
	
	TEST_INLINE() = [&]() {
		cllio::mem_stream_read ms(buffer.data(), buffer.size());
		run_read_functions_f1(ms);
	};

	TEST_INLINE() = [&]() {
		cllio::mem_stream_read ms(buffer.data(), buffer.size());
		run_read_functions_f2(ms);
	};
	
	TEST_INLINE() = [&]() {
		cllio::mem_stream_read ms(buffer.data(), buffer.size());
		run_read_functions_f3(ms);
	};

	TEST_INLINE() = [&]() {
		cllio::mem_stream_read ms(buffer.data(), buffer.size());
		run_read_functions_f4(ms);
	};
}

void run_main_tests()
{
	const std::size_t sample_file_expected_size = 1134;

	TEST_INLINE() = [&]() {
		// write sample file
		cllio::std_file_write out;
		out.create(file_path, true, false);
		test_writer(out);
	};

	TEST_INLINE() = [&]() {
		// get output size
		cllio::size_info f;
		test_writer(f);
		TEST_ASSERT(f.size() == sample_file_expected_size);
		/*if (f.size() != sample_file_expected_size)
		{
			std::cerr << "Error: cllio::size_info size mismatch.\n";
			return false;
		}*/
	};

	TEST_FUNCTION(TestFileReaders);
	
	TEST_INLINE() = [&]() {
		// memory test
		std::vector<cllio::byte_t> buffer;

		TEST_INLINE() = [&]() {
			cllio::std_file_read in;
			in.open(ref_file_path, true);
			TEST_ASSERT(in.isOpen());
			/*if (in.isOpen() == false)
			{
				std::cerr << "Failed open test file.\n";
				return false;
			}*/
			TEST_ASSERT(in.get_file_size() == sample_file_expected_size);
			/*if (in.get_file_size() != sample_file_expected_size)
			{
				std::cerr << "Error: Test file Size mismatch.\n";
				return false;
			}*/
			in.read_into_container(buffer);
			TEST_ASSERT(buffer.size() == sample_file_expected_size);
			/*if (buffer.size() != sample_file_expected_size)
			{
				std::cerr << "Failed to read entire file into container.\n";
				return false;
			}*/
			in.close();
			TEST_ASSERT(in.isOpen() == false);
			/*{
				std::cerr << "Error: Input file should be closed.\n";
				return false;
			}*/
		};

		TEST_INLINE() = [&]() {
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
		};

		TEST_INLINE() = [&]() {
			TestMemoryReaders(buffer);
		};
		

		TEST_INLINE() = [&]() {
			// since there is no normal way to check mem_stream_write_unchecked UB we reserve a double sized buffer for this
			// we then run the write operations and see if they match
			std::vector<cllio::byte_t> extended_buffer;
			extended_buffer.resize(buffer.size() * 2);
			std::memcpy(extended_buffer.data(), buffer.data(), buffer.size());
			cllio::mem_stream_write_unchecked writer(extended_buffer.data());
			test_writer(writer);
			bool data_ok = std::memcmp(extended_buffer.data(), buffer.data(), buffer.size()) == 0;
			bool iterator_ok = writer.data() == (extended_buffer.data() + buffer.size());
			TEST_ASSERT(data_ok && iterator_ok);
			/*{
				std::cerr << "Error: cllio::mem_stream_write_unchecked data or iterator mismatch.\n";
				return false;
			}*/
		};

		TEST_INLINE() = [&]() {
			std::vector<cllio::byte_t> test_buffer;
			test_buffer.resize(buffer.size());
			cllio::mem_stream_write writer(test_buffer.data(), test_buffer.size());
			test_writer(writer);
			bool data_ok = std::memcmp(test_buffer.data(), buffer.data(), buffer.size()) == 0;
			bool iterator_ok = writer.begin() == (test_buffer.data() + buffer.size());
			TEST_ASSERT(data_ok && iterator_ok);
			/*{
				std::cerr << "Error: cllio::mem_stream_write data or iterator mismatch.\n";
				return false;
			}*/
		};

		TEST_INLINE() = [&]() {
			std::vector<cllio::byte_t> test_buffer;
			auto					   functor_writer = cllio::memory_functor_write<std::function<cllio::byte_t* (const std::size_t)>>{ [&](const std::size_t ns) {
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
		};
	};

}

void test_utils()
{
	auto check_value = [](const uint64_t i) {
		char buffer[16];
		std::memset(&buffer[0], 0, 16);

		uint64_t				ref = i;
		cllio::mem_stream_write w(&buffer[4], 8);
		if (cllio::utils::write_packed_uint64_t(w, ref) == false)
			return false;

		uint64_t			   check = std::numeric_limits<uint64_t>::max();
		cllio::mem_stream_read r(&buffer[4], 8);
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
		TEST_ASSERT(check_value(value));
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

TEST_MAIN(test_main);
