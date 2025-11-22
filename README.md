
# About

CLLIO (c++ low level io) wants to be a lightweight, ZERO warnings stream io and serialization abstraction layer.

What cllio does not have:
- no complex serialization code.
- direct memory allocations
- complex buffering logic

What it should have:
- 0 Warnings (W4) on "all" compilers
- minimal effort to integrate into project
- usable functions without the need to write boilerplate
- *EXTREME* error checking without performance sacrifice
- no template overkill, functions should be easy to read with a debugger.

Status:
- `std::FILE` read/write layer: **Working**
- `void*` memory read/write layer: **Working**

0 warnings (using premake5 warnings("Extra")):
- `Visual studio 2019` *0 warnings*
- `docker/ubuntu` *0 warnings*

# Base Classes:

- `memory_rstream` reads from `const void*`
- `memory_rstream_unchecked` reads from `const void*` no end() checks
- `memory_wstream` writes to `void*`
- `memory_wstream_unchecked` writes to `void*` no end() checks
- `memory_rfunc<F>` reads from a return value of a functor `const cllio::byte_t* F::operator(std::size_t)`
- `memory_wfunc<F>` writes to functor result of `cllio::byte_t* F::operator(std::size_t)`
- `stdfile_rstream_view` read from `std::FILE*`, does not own the file
- `stdfile_wstream_view` write to `std::FILE*`, does not own the file
- `stdfile_rstream` read from `std::FILE*`, owns file handle
- `stdfile_wstream` write from `std::FILE*`, owns file handle

# Helper classes

- `size_info` calculates write size
- `call_trace_info` debug utility, prints all calls to serialization primitives
- `file_read_mapview` maps file to `const void*`, use `data()` and `size()`
- `tcpsocket` for simple socket io

# Other

- `memory_vector_serializer<V>` typedef for writer to a std::vector like container (`size()/resize()`)

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

1. `T 	 	 pop_T();` Try to read, runtime assert and UB on failure.
2. `bool 	 pop_T(T & out);`  Try to read, returns false on failure and `out` is unchanged
3. `T 	 	 pop_T(bool& error);` Try to read, returns T(0) on error and error = true.
4. `T 	 	 pop_default_T(const T & default);` Try to read, return `default` on failure.
5. `bool 	 pop_default_T(T & out, const T & default);` Try to read, `out = default` on failure and returns false.
6. `void 	 pop_T(T & out, bool& error);`  Try to read if 'error == false', on filiure 'error = true'
7. `void	 read_raw_buffer(void * dest, const std::size_t byte_count);` Try to read, UB on failiure
8. `bool	 tryread_raw_buffer(void * dest, const std::size_t byte_count);` If read is not possible function returns false, otherwise it populates 'dest' buffer

Where `T` is one of 'Supported Types'.

Note: one some Undefined behaviors there is an assert

# Binary Write functions

1. `void push_T(const T & value)` will write; UB on failure.
2. `bool try_push_T(const T & value)` will return false if push failed
3. `void push_ptr(const void *)` will write; UB on failure.
4. `bool try_push_ptr(const void*)` will return false if push failed
5. `void write_raw_buffer(const void * data, const std::size_t byte_count);` Try to write, UB on failiure
6. `bool trywrite_raw_buffer(const void * data, const std::size_t byte_count);` Try to write, Returns false on failiure


Where `T` is one of 'Supported Types'.
