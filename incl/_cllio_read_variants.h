

// T 	pop_default_T(const T & default);
inline uint8_t pop_default_uint8(const uint8_t _default)
{
	uint8_t r;
	if (pop_uint8(r))
		return r;
	return _default;
}

inline uint16_t pop_default_uint16(const uint16_t _default)
{
	uint16_t r;
	if (pop_uint16(r))
		return r;
	return _default;
}

inline uint32_t pop_default_uint32(const uint32_t _default)
{
	uint32_t r;
	if (pop_uint32(r))
		return r;
	return _default;
}

inline uint64_t pop_default_uint64(const uint64_t _default)
{
	uint64_t r;
	if (pop_uint64(r))
		return r;
	return _default;
}

inline int8_t pop_default_int8(const int8_t _default)
{
	int8_t r;
	if (pop_int8(r))
		return r;
	return _default;
}

inline int16_t pop_default_int16(const int16_t _default)
{
	int16_t r;
	if (pop_int16(r))
		return r;
	return _default;
}

inline int32_t pop_default_int32(const int32_t _default)
{
	int32_t r;
	if (pop_int32(r))
		return r;
	return _default;
}

inline int64_t pop_default_int64(const int64_t _default)
{
	int64_t r;
	if (pop_int64(r))
		return r;
	return _default;
}

inline float pop_default_float(const float _default)
{
	float r;
	if (pop_float(r))
		return r;
	return _default;
}

inline double pop_default_double(const double _default)
{
	double r;
	if (pop_double(r))
		return r;
	return _default;
}

//--------------------------------------------------------------------------------------------------------------------------------

// T 	pop_default_T(T& out, const T & default);

inline bool pop_default_uint8(uint8_t& out, const uint8_t _default)
{
	if (pop_uint8(out))
		return true;
	out = _default;
	return false;
}
inline bool pop_default_uint16(uint16_t& out, const uint16_t _default)
{
	if (pop_uint16(out))
		return true;
	out = _default;
	return false;
}
inline bool pop_default_uint32(uint32_t& out, const uint32_t _default)
{
	if (pop_uint32(out))
		return true;
	out = _default;
	return false;
}
inline bool pop_default_uint64(uint64_t& out, const uint64_t _default)
{
	if (pop_uint64(out))
		return true;
	out = _default;
	return false;
}
inline bool pop_default_int8(int8_t& out, const int8_t _default)
{
	if (pop_int8(out))
		return true;
	out = _default;
	return false;
}
inline bool pop_default_int16(int16_t& out, const int16_t _default)
{
	if (pop_int16(out))
		return true;
	out = _default;
	return false;
}
inline bool pop_default_int32(int32_t& out, const int32_t _default)
{
	if (pop_int32(out))
		return true;
	out = _default;
	return false;
}
inline bool pop_default_int64(int64_t& out, const int64_t _default)
{
	if (pop_int64(out))
		return true;
	out = _default;
	return false;
}
inline bool pop_default_float(float& out, const float _default)
{
	if (pop_float(out))
		return true;
	out = _default;
	return false;
}
inline bool pop_default_double(double& out, const double _default)
{
	if (pop_double(out))
		return true;
	out = _default;
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

// void pop_T(T & out, bool& error);

inline void pop_uint8(uint8_t& out, bool& error)
{
	error = error || pop_uint8(out) == false;
}
inline void pop_uint16(uint16_t& out, bool& error)
{
	error = error || pop_uint16(out) == false;
}
inline void pop_uint32(uint32_t& out, bool& error)
{
	error = error || pop_uint32(out) == false;
}
inline void pop_uint64(uint64_t& out, bool& error)
{
	error = error || pop_uint64(out) == false;
}
inline void pop_int8(int8_t& out, bool& error)
{
	error = error || pop_int8(out) == false;
}
inline void pop_int16(int16_t& out, bool& error)
{
	error = error || pop_int16(out) == false;
}
inline void pop_int32(int32_t& out, bool& error)
{
	error = error || pop_int32(out) == false;
}
inline void pop_int64(int64_t& out, bool& error)
{
	error = error || pop_int64(out) == false;
}
inline void pop_float(float& out, bool& error)
{
	error = error || pop_float(out) == false;
}
inline void pop_double(double& out, bool& error)
{
	error = error || pop_double(out) == false;
}

//--------------------------------------------------------------------------------------------------------------------------------

// T pop_T(bool& error);
inline uint8_t pop_uint8(bool& error)
{
	uint8_t r = uint8_t(0);
	error = error || pop_uint8(r) == false;
	return r;
}

inline uint16_t pop_uint16(bool& error)
{
	uint16_t r = uint16_t(0);
	error = error || pop_uint16(r) == false;
	return r;
}

inline uint32_t pop_uint32(bool& error)
{
	uint32_t r = uint32_t(0);
	error = error || pop_uint32(r) == false;
	return r;
}

inline uint64_t pop_uint64(bool& error)
{
	uint64_t r = uint64_t(0);
	error = error || pop_uint64(r) == false;
	return r;
}

inline int8_t pop_int8(bool& error)
{
	int8_t r = int8_t(0);
	error = error || pop_int8(r) == false;
	return r;
}

inline int16_t pop_int16(bool& error)
{
	int16_t r = int16_t(0);
	error = error || pop_int16(r) == false;
	return r;
}

inline int32_t pop_int32(bool& error)
{
	int32_t r = int32_t(0);
	error = error || pop_int32(r) == false;
	return r;
}

inline int64_t pop_int64(bool& error)
{
	int64_t r = int64_t(0);
	error = error || pop_int64(r) == false;
	return r;
}

inline float pop_float(bool& error)
{
	float r = float(0);
	error = error || pop_float(r) == false;
	return r;
}

inline double pop_double(bool& error)
{
	double r = double(0);
	error = error || pop_double(r) == false;
	return r;
}

//--------------------------------------------------------------------------------------------------------------------------------
