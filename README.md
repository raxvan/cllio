
# About

*This is in early development*

CLLIO (c++ low level io) wants to be a lightweight, ZERO warnings (w4) stream io abstraction layer. It also accommodates different styles of code.

Status:
- `std::FILE` read/write layer: **Done**
- `void*` read/write layer: *todo*
- `socket` read/write layer: *todo*

Tested:
- `Visual studio 2019` *0 warnings*
- `docker ubuntu` *todo*
- `xcode` *todo*


# Compile Defines

When `DEBUG` is defined asserts are enabled by default.

1. `CLLIO_CAST_FUNCTION 0` (default) uint32_t from/to uint8_t[4] is done with byte stitching.
2. `CLLIO_CAST_FUNCTION 1` uint32_t from/to uint8_t[4] is done with `std::memcpy`.
2. `CLLIO_CAST_FUNCTION 2` uint32_t from/to uint8_t[4] is done with `union`.

3. `CLLIO_FORCE_ASSERT 0` disable asserts for debug
4. `CLLIO_FORCE_ASSERT 1` enable asserts for release
5. `CLLIO_FORCE_ASSERT 2` enable asserts without handling function. Code expects to link with: 
```
namespace cllio
{
	extern void cllio_assert_failed(const char * file, const int line, const char * cond);
}
```

# Binary Read functions
Readers follow the following format:

1. `bool 	 pop_T(T & out);`  Try to read, returns false on failure.
2. `T 	 	 pop_T();` Try to read, runtime assert and UB on failure.
3. `T 	 	 popdefault_T(const T & default);` Try to read, return `default` on failure.
4. `bool 	 popdefault_T(T & out, const T & default);` Try to read, `out = default` on failure and returns true/false.

where T is :

- `uint8` with `uint8_t`
- `uint16` with `uint16_t`
- `uint32` with `uint32_t`
- `uint64` with `uint64_t`
- `int8` with `int8_t`
- `int16` with `int16_t`
- `int32` with `int32_t`
- `int64` with `int64_t`
- `f32` with `float`
- `f64` with `double`

# Binary Write functions

Same as read functions for all `T`:

1. `void push_T(const T & value)` will write; UB on failure.