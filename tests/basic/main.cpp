
#include <cllio.h>
#include <limits>
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <functional>
#include <cllio_tools/file_read_mapview.h>
#include <cllio_tools/write_size.h>
#include <cllio_tools/packing.h>

std::ostringstream str_dump;
const auto		   ref_file_path = "C:/Users/razvano/Desktop/wat/wk/trunk/workspace/cllio/tests/samples.bin";
const auto		   file_path = "samples.bin";
std::size_t		   failed = 0;
std::size_t		   passed = 0;

void on_test_failed()
{
	failed++;
}
void on_test_passed()
{
	passed++;
}

#define TEST_ASSUME(_COND)                                                                                 \
	[&]() -> std::ostream& {                                                                               \
		if (!(_COND))                                                                                      \
		{                                                                                                  \
			std::cerr << "FAILED:" << #_COND << " at " << __FILE__ << "(" << __LINE__ << ")" << std::endl; \
			on_test_failed();                                                                              \
			return std::cerr;                                                                              \
		}                                                                                                  \
		return str_dump;                                                                                   \
	}()

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
void tests_basic_write(T& stream)
{
#define TEST_ITEM(TYPE, VALUE) stream.push_##TYPE(VALUE)
	[&]() {
#include "test_set.h"
	}();

#undef TEST_ITEM
}

template <class T>
void run_read_functions_f0(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                                                                                  \
	{                                                                                                           \
		TYPE v = stream.pop_##TYPE();                                                                           \
		if ((VALUE) != v)                                                                                       \
		{                                                                                                       \
			std::cerr << "FAILED " #TYPE " pop_" #TYPE "() with value " #VALUE << "/result:" << v << std::endl; \
			on_test_failed();                                                                                   \
		}                                                                                                       \
		else                                                                                                    \
		{                                                                                                       \
			std::cout << "OK " #TYPE " pop_" #TYPE "() with value " << #VALUE << std::endl;                     \
			on_test_passed();                                                                                   \
		}                                                                                                       \
	}

	[&]() {
#include "test_set.h"
	}();

#undef TEST_ITEM
}

template <class T>
void run_read_functions_f1(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                                                                                                          \
	{                                                                                                                                   \
		TYPE value = not_value(VALUE);                                                                                                  \
		bool e = stream.pop_##TYPE(value);                                                                                              \
		if ((VALUE) != value || e == false)                                                                                             \
		{                                                                                                                               \
			std::cerr << "FAILED bool pop_" #TYPE "(" #TYPE "&) with value " #VALUE << "/value=" << value << "/err:" << e << std::endl; \
			on_test_failed();                                                                                                           \
		}                                                                                                                               \
		else                                                                                                                            \
		{                                                                                                                               \
			std::cout << "OK bool pop_" #TYPE "(" #TYPE "&) with value " << #VALUE << std::endl;                                        \
			on_test_passed();                                                                                                           \
		}                                                                                                                               \
	}

	[&]() {
#include "test_set.h"
	}();

#undef TEST_ITEM
}
template <class T>
void run_read_functions_f2(T& stream)
{
#define TEST_ITEM(TYPE, VALUE)                                                                                                          \
	{                                                                                                                                   \
		bool e = false;                                                                                                                 \
		TYPE value = stream.pop_##TYPE(e);                                                                                              \
		if ((VALUE) != value || e == true)                                                                                              \
		{                                                                                                                               \
			std::cerr << "FAILED " #TYPE " pop_" #TYPE "(bool&) with value " #VALUE << "/value=" << value << "/err:" << e << std::endl; \
			on_test_failed();                                                                                                           \
		}                                                                                                                               \
		else                                                                                                                            \
		{                                                                                                                               \
			std::cout << "OK " #TYPE " pop_" #TYPE "(bool&) with value " #VALUE << std::endl;                                           \
			on_test_passed();                                                                                                           \
		}                                                                                                                               \
	}

	[&]() {
#include "test_set.h"
	}();

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
			on_test_failed();                                                                                              \
			std::cerr << "FAILED bool popdefault_" #TYPE "(" #TYPE "&, const " #TYPE "&) with value " #VALUE << std::endl; \
		}                                                                                                                  \
		else                                                                                                               \
		{                                                                                                                  \
			std::cout << "OK bool popdefault_" #TYPE "(" #TYPE "&, const " #TYPE "&) with value " << #VALUE << std::endl;  \
			on_test_passed();                                                                                              \
		}                                                                                                                  \
	}

	[&]() {
#include "test_set.h"
	}();

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
			on_test_failed();                                                                                       \
			std::cerr << "FAILED " #TYPE " popdefault_" #TYPE "(const " #TYPE "&) with value " #VALUE << std::endl; \
		}                                                                                                           \
		else                                                                                                        \
		{                                                                                                           \
			std::cout << "OK " #TYPE " popdefault_" #TYPE "(const " #TYPE "&) with value " << #VALUE << std::endl;  \
			on_test_passed();                                                                                       \
		}                                                                                                           \
	}

	[&]() {
#include "test_set.h"
	}();

#undef TEST_ITEM
}

void TestFileReaders()
{
	{
		cllio::stdfile_rstream in;
		in.open(ref_file_path, true);
		run_read_functions_f0(in);
	}
	{
		cllio::stdfile_rstream in;
		in.open(ref_file_path, true);
		run_read_functions_f1(in);
	}
	{
		cllio::stdfile_rstream in;
		in.open(ref_file_path, true);
		run_read_functions_f2(in);
	}
	{
		cllio::stdfile_rstream in;
		in.open(ref_file_path, true);
		run_read_functions_f3(in);
	}
	{
		cllio::stdfile_rstream in;
		in.open(ref_file_path, true);
		run_read_functions_f4(in);
	}
}
void TestMemoryReaders(const std::vector<cllio::byte_t>& ref_data)
{
	{
		cllio::memory_rstream_unchecked ms(ref_data.data());
		run_read_functions_f0(ms);
	}

	{
		cllio::memory_rstream ms(ref_data.data(), ref_data.size());
		run_read_functions_f0(ms);
	}
	{
		cllio::memory_rstream ms(ref_data.data(), ref_data.size());
		run_read_functions_f1(ms);
	}
	{
		cllio::memory_rstream ms(ref_data.data(), ref_data.size());
		run_read_functions_f2(ms);
	}
	{
		cllio::memory_rstream ms(ref_data.data(), ref_data.size());
		run_read_functions_f3(ms);
	}
	{
		cllio::memory_rstream ms(ref_data.data(), ref_data.size());
		run_read_functions_f4(ms);
	}
}

void TestMemoryReadersIntegrity(const std::vector<cllio::byte_t>& ref_data)
{
	{
		// since there is no normal way to check memory_wstream_unchecked UB we reserve a double sized buffer for this
		// we then run the write operations and see if they match

		// make ref data copy
		std::vector<cllio::byte_t> work_buffer;
		work_buffer.resize(ref_data.size() * 2);

		cllio::memory_wstream_unchecked writer(work_buffer.data());
		tests_basic_write(writer);

		auto data_ok = std::memcmp(work_buffer.data(), ref_data.data(), ref_data.size());
		bool iterator_ok = writer.data() == (work_buffer.data() + ref_data.size());
		TEST_ASSUME(data_ok == 0) << "Data mismatch at:" << data_ok << std::endl;
		TEST_ASSUME(iterator_ok);
	}
	{
		std::vector<cllio::byte_t> test_buffer;
		test_buffer.resize(ref_data.size());

		cllio::memory_wstream writer(test_buffer.data(), test_buffer.size());
		tests_basic_write(writer);

		bool data_ok = std::memcmp(test_buffer.data(), ref_data.data(), ref_data.size()) == 0;
		bool iterator_ok = writer.begin() == (test_buffer.data() + ref_data.size());
		TEST_ASSUME(data_ok);
		TEST_ASSUME(iterator_ok);
	}
}

bool run_main_tests()
{
	std::size_t sample_file_expected_size = 1219;
	{
		// write sample file
		cllio::stdfile_wstream out;
		out.create(file_path, true, false);
		tests_basic_write(out);
	}

	{
		// get output size
		cllio::write_size f;
		tests_basic_write(f);

		TEST_ASSUME(f.size() == sample_file_expected_size) << "Error: cllio::size_info size mismatch;" << f.size() << std::endl;
	}

	{
		TestFileReaders();
	}

	{
		// memory test
		std::vector<cllio::byte_t> buffer;

		{
			cllio::stdfile_rstream in;
			in.open(ref_file_path, true);
			TEST_ASSUME(in.isOpen() == true) << "Failed open test file." << std::endl;
			TEST_ASSUME(in.get_file_size() == sample_file_expected_size) << "Error: Test file Size mismatch." << std::endl;

			in.read_into_container(buffer);
			TEST_ASSUME(buffer.size() == sample_file_expected_size) << "Failed to read entire file into container." << std::endl;

			in.close();
			TEST_ASSUME(in.isOpen() == false) << "Input file should be closed." << std::endl;
		}

		{
			cllio::file_read_mapview in(ref_file_path);
			TEST_ASSUME(in.size() == buffer.size()) << "cllio::file_read_mapview size mismatch" << std::endl;

			bool data_ok = std::memcmp(in.data(), in.data(), buffer.size()) == 0;
			TEST_ASSUME(data_ok == true) << "cllio::file_read_mapview data mismatch." << std::endl;
		}

		TestMemoryReaders(buffer);

		TestMemoryReadersIntegrity(buffer);
		
		{
			std::vector<cllio::byte_t> test_buffer;
			auto					   functor_writer = cllio::memory_wfunc<std::function<cllio::byte_t*(const std::size_t)>> { [&](const std::size_t ns) {
				auto				   sz = test_buffer.size();
				test_buffer.resize(sz + ns);
				return test_buffer.data() + sz;
			} };

			tests_basic_write(functor_writer);
			bool size_ok = test_buffer.size() == buffer.size();
			bool data_ok = std::memcmp(test_buffer.data(), buffer.data(), buffer.size()) == 0;
			TEST_ASSUME(size_ok);
			TEST_ASSUME(data_ok);
		}
	}

	if (failed)
	{
		std::cerr << "\ntest:FAILURE\n";
		return false;
	}
	else
	{
		std::cout << "\ntest:SUCCESS\n";
		return true;
	}
}

bool test_utils()
{
	auto check_value = [](const uint64_t i) {
		char buffer[16];
		std::memset(&buffer[0], 0, 16);

		uint64_t			  ref = i;
		cllio::memory_wstream w(&buffer[4], 8);
		if (cllio::pack::write_packed_uint64_t(w, ref) == false)
			return false;

		uint64_t			  check = std::numeric_limits<uint64_t>::max();
		cllio::memory_rstream r(&buffer[4], 8);
		if (cllio::pack::read_packed_uint64_t(r, check) == false)
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
		{
			on_test_failed();
			std::cerr << "packed uint64 read/write failed with value " << value << std::endl;
		}
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

	return true;
}

int main(int argc, const char** argv)
{
#ifdef DEBUG
	std::cout << "arg count " << argc << std::endl;
	for (int i = 0; i < argc; i++)
		std::cout << "arg[" << i << "]=" << argv[i] << std::endl;
#endif

	if (argc == 1)
	{
		if (run_main_tests() == false)
			return -1;

		if (test_utils() == false)
			return -1;
	}

	return 0;
}
