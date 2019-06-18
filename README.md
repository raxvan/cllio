
# About

CLLIO (c++ low level io) wants to be a lightweight, ZERO warnings stream io abstraction layer. It also accommodates different coding styles.

What cllio does not have:
- complex serialization; no templates to write/read vectors or structures
- direct memory allocations
- Little/Big Endian cross compatibility (right now, but this will be handled in the future)
- complex buffering logic

What it should have:
- 0 Warnings (W4) on "all" compilers
- fast compile times, with minimal effort to integrate into project
- usable functions without the need to write boilerplate
- error checking to the bone without performance sacrifice
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
- `size_info` calculates write size
- `std_file_read_view` read from `std::FILE*`
- `std_file_write_view` write to `std::FILE*`
- `std_file_read` read from `std::FILE*`, owns file handle
- `std_file_write` write from `std::FILE*`, owns file handle

# Binary Read functions
Readers have functions that follow the following format:

1. `bool 	 pop_T(T & out);`  Try to read, returns false on failure.
2. `T 	 	 pop_T();` Try to read, runtime assert and UB on failure.
3. `T 	 	 popdefault_T(const T & default);` Try to read, return `default` on failure.
4. `bool 	 popdefault_T(T & out, const T & default);` Try to read, `out = default` on failure and returns true/false.
5. `void	 buffer_read(void * dest, const std::size_t ammount);` Try to read, UB on failiure

where T is :

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

# Binary Write functions
Same as read functions for all `T`:

1. `void push_T(const T & value)` will write; UB on failure.
2. `void buffer_write(const void * data, const std::size_t byte_count);` Try to write, UB on failiure

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