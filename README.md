
# About

CLLIO (c++ low level io) wants to be a lightweight, ZERO warnings stream io and serialization abstraction layer.

What cllio does not have:
- complex serialization; no templates to write/read vectors or structures
- direct memory allocations
- complex buffering logic

What it should have:
- 0 Warnings (W4) on "all" compilers
- fast compile times, with minimal effort to integrate into project
- usable functions without the need to write boilerplate
- *EXTREME* error checking without performance sacrifice
- no template overkill, functions should be easy to read with a debugger.

Status:
- `std::FILE` read/write layer: **Working**
- `void*` memory read/write layer: **Working**

0 warnings (using premake5 warnings("Extra")):
- `Visual studio 2019` *0 warnings*
- `docker/ubuntu` *0 warnings*

TODO:
- xcode/macos testing *todo*
- running static code analyzers: *todo*
- LE, BE support

# Classes:

- `mem_stream_read` reads from `const void*`
- `mem_stream_read_unchecked` reads from `const void*` no end() checks
- `mem_stream_write` writes to `void*`
- `mem_stream_write_unchecked` writes to `void*` no end() checks
- `memory_functor_write<F>` writes to functor result of `cllio::byte_t* F::operator(std::size_t)`
- `std_file_read_view` read from `std::FILE*`
- `std_file_write_view` write to `std::FILE*`
- `std_file_read` read from `std::FILE*`, owns file handle
- `std_file_write` write from `std::FILE*`, owns file handle
- `size_info` calculates write size
- `call_trace_info` debug

Note: calling any read/write functions on a closed `std_file_*` is UB;

# Supported Types:

Read/Write functions can handle following types:

- `uint8_t`
- `uint16_t`
- `uint32_t`
- `uint64_t`
- `int8_t`
- `int16_t`
- `int32_t`
- `int64_t`
- `float`
- `double`

# Binary Read functions
Readers have functions that follow the following format:

1. `bool 	 pop_T(T & out);`  Try to read, returns false on failure.
2. `T 	 	 pop_T();` Try to read, runtime assert and UB on failure.
3. `T 	 	 pop_T(bool& error);` Try to read, returns T(0) on error and error = true.
4. `T 	 	 popdefault_T(const T & default);` Try to read, return `default` on failure.
5. `bool 	 popdefault_T(T & out, const T & default);` Try to read, `out = default` on failure and returns true/false.
6. `void 	 pop_T(T & out, bool& error);`  Try to read if 'error == false', on filiure 'error = true'
7. `void	 read_raw_buffer(void * dest, const std::size_t byte_count);` Try to read, UB on failiure
8. `bool	 tryread_raw_buffer(void * dest, const std::size_t byte_count);` If read is not possible function returns false, otherwise it populates 'dest' buffer

Where `T` is one of 'Supported Types'.


# Binary Write functions

1. `void push_T(const T & value)` will write; UB on failure.
2. `bool trypush_T(const T & value)` will return false if push failed
3. `void push_ptr(const void *)` will write; UB on failure.
4. `bool trypush_ptr(const void*)` will return false if push failed
5. `void push_raw_buffer(const void * data, const std::size_t byte_count);` Try to write, UB on failiure
6. `bool trypush_raw_buffer(const void * data, const std::size_t byte_count);` Try to write, Returns false on failiure


Where `T` is one of 'Supported Types'.

# Compile Defines

When `DEBUG` is defined asserts are enabled by default.

1. `CLLIO_FORCE_ASSERT 0` disable asserts for debug
2. `CLLIO_FORCE_ASSERT 1` enable asserts for release
3. `CLLIO_FORCE_ASSERT 2` enable asserts without handling function. Code expects to link with:
```
namespace cllio
{
	extern void cllio_assert_failed(const char * file, const int line, const char * cond);
}
```
