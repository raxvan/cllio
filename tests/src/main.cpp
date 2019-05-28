

#include <cllio.h>
#include <limits>
#include <iostream>

std::size_t failed = 0;
template <class T>
T not_value(const T & value)
{
	union {
		uint8_t data[sizeof(T)];
		T 		value;
	} buffer;
	buffer.value = value;
	for(std::size_t i = 0; i < sizeof(T);i++)
		buffer.data[i] = ~buffer.data[i];
	return buffer.value;
}


template <class T>
void setup_test_value(T & stream)
{
	#define TEST_ITEM(TYPE,VALUE) stream.push_##TYPE( VALUE )

	#include "test_set.h"

	#undef TEST_ITEM
}

template <class T>
void test_value_f0(T & stream)
{
	#define TEST_ITEM(TYPE,VALUE) { TYPE v = stream.pop_##TYPE(); \
		if((VALUE) != v)  \
			{ failed ++; std::cout << "FAILED " #TYPE "pop_" #TYPE "() with value " #VALUE << std::endl; } \
		else \
			std::cout << "OK " #TYPE " pop_" #TYPE "() with value " << #VALUE << std::endl; \
	}

	#include "test_set.h"

	#undef TEST_ITEM
}

template <class T>
void test_value_f1(T & stream)
{
	#define TEST_ITEM(TYPE,VALUE) { TYPE value = not_value(VALUE); bool f = stream.pop_##TYPE(value); \
		if((VALUE) != value || f == false)  \
			{ failed ++; std::cout << "FAILED bool pop_" #TYPE "(" #TYPE "&) with value " #VALUE << std::endl; } \
		else \
			std::cout << "OK bool pop_" #TYPE "(" #TYPE "&) with value " << #VALUE << std::endl; \
	}

	#include "test_set.h"

	#undef TEST_ITEM
}
template <class T>
void test_value_f2(T & stream)
{
	#define TEST_ITEM(TYPE,VALUE) { TYPE value = not_value(VALUE); bool f = stream.popdefault_##TYPE(value,not_value(VALUE)); \
		if((VALUE) != value || f == false)  \
			{ failed ++; std::cout << "FAILED bool popdefault_" #TYPE "(" #TYPE "&, const " #TYPE "&) with value " #VALUE << std::endl; } \
		else \
			std::cout << "OK bool popdefault_" #TYPE "(" #TYPE "&, const " #TYPE "&) with value " << #VALUE << std::endl; \
	}

	#include "test_set.h"

	#undef TEST_ITEM
}
template <class T>
void test_value_f3(T & stream)
{
	#define TEST_ITEM(TYPE,VALUE) { TYPE value = stream.popdefault_##TYPE(not_value(VALUE)); \
		if((VALUE) != value)  \
			{ failed ++; std::cout << "FAILED " #TYPE " popdefault_" #TYPE "(const " #TYPE "&) with value " #VALUE << std::endl; } \
		else \
			std::cout << "OK " #TYPE " popdefault_" #TYPE "(const " #TYPE "&) with value " << #VALUE << std::endl; \
	}

	#include "test_set.h"

	#undef TEST_ITEM
}

int main()
{
	{
		cllio::std_file_writer out;
		out.open("samples.bin",true,false);
		setup_test_value(out);
	}
	{
		cllio::std_file_reader in;
		in.open("samples.bin",true);
		test_value_f0(in);
	}
	{
		cllio::std_file_reader in;
		in.open("samples.bin",true);
		test_value_f1(in);
	}
	{
		cllio::std_file_reader in;
		in.open("samples.bin",true);
		test_value_f2(in);
	}
	{
		cllio::std_file_reader in;
		in.open("samples.bin",true);
		test_value_f3(in);
	}

	if (failed)
		std::cout << "\nFAILED:" << failed << std::endl;
	else
		std::cout << "\nOK\n";
	return 0;
	
}