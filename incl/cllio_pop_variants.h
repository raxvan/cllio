

// T 	popdefault_T(const T & default);
inline uint8_t popdefault_uint8_t(const uint8_t _default)
{
	uint8_t r;
	if(pop_uint8_t(r))
		return r;
	return _default;
}

inline uint16_t popdefault_uint16_t(const uint16_t _default)
{
	uint16_t r;
	if(pop_uint16_t(r))
		return r;
	return _default;
}

inline uint32_t popdefault_uint32_t(const uint32_t _default)
{
	uint32_t r;
	if(pop_uint32_t(r))
		return r;
	return _default;
}

inline uint64_t popdefault_uint64_t(const uint64_t _default)
{
	uint64_t r;
	if(pop_uint64_t(r))
		return r;
	return _default;
}

inline int8_t popdefault_int8_t(const int8_t _default)
{
	int8_t r;
	if(pop_int8_t(r))
		return r;
	return _default;
}

inline int16_t popdefault_int16_t(const int16_t _default)
{
	int16_t r;
	if(pop_int16_t(r))
		return r;
	return _default;
}

inline int32_t popdefault_int32_t(const int32_t _default)
{
	int32_t r;
	if(pop_int32_t(r))
		return r;
	return _default;
}

inline int64_t popdefault_int64_t(const int64_t _default)
{
	int64_t r;
	if(pop_int64_t(r))
		return r;
	return _default;
}

inline float popdefault_float(const float _default)
{
	float r;
	if(pop_float(r))
		return r;
	return _default;
}

inline double popdefault_double(const double _default)
{
	double r;
	if(pop_double(r))
		return r;
	return _default;
}

//--------------------------------------------------------------------------------------------------------------------------------



// T 	popdefault_T(T& out, const T & default);

inline bool popdefault_uint8_t(uint8_t& out, const uint8_t _default)
{
	if(pop_uint8_t(out))
		return true;
	out = _default;
	return false;
}
inline bool popdefault_uint16_t(uint16_t& out, const uint16_t _default)
{
	if(pop_uint16_t(out))
		return true;
	out = _default;
	return false;
}
inline bool popdefault_uint32_t(uint32_t& out, const uint32_t _default)
{
	if(pop_uint32_t(out))
		return true;
	out = _default;
	return false;
}
inline bool popdefault_uint64_t(uint64_t& out, const uint64_t _default)
{
	if(pop_uint64_t(out))
		return true;
	out = _default;
	return false;
}
inline bool popdefault_int8_t(int8_t& out, const int8_t _default)
{
	if(pop_int8_t(out))
		return true;
	out = _default;
	return false;
}
inline bool popdefault_int16_t(int16_t& out, const int16_t _default)
{
	if(pop_int16_t(out))
		return true;
	out = _default;
	return false;
}
inline bool popdefault_int32_t(int32_t& out, const int32_t _default)
{
	if(pop_int32_t(out))
		return true;
	out = _default;
	return false;
}
inline bool popdefault_int64_t(int64_t& out, const int64_t _default)
{
	if(pop_int64_t(out))
		return true;
	out = _default;
	return false;
}
inline bool popdefault_float(float& out, const float _default)
{
	if(pop_float(out))
		return true;
	out = _default;
	return false;
}
inline bool popdefault_double(double& out, const double _default)
{
	if(pop_double(out))
		return true;
	out = _default;
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------

// void pop_T(T & out, bool& error);

inline void pop_uint8_t(uint8_t& out, bool& error)
{
	error = error || pop_uint8_t(out);
}
inline void pop_uint16_t(uint16_t& out, bool& error)
{
	error = error || pop_uint16_t(out);
}
inline void pop_uint32_t(uint32_t& out, bool& error)
{
	error = error || pop_uint32_t(out);
}
inline void pop_uint64_t(uint64_t& out, bool& error)
{
	error = error || pop_uint64_t(out);
}
inline void pop_int8_t(int8_t& out, bool& error)
{
	error = error || pop_int8_t(out);
}
inline void pop_int16_t(int16_t& out, bool& error)
{
	error = error || pop_int16_t(out);
}
inline void pop_int32_t(int32_t& out, bool& error)
{
	error = error || pop_int32_t(out);
}
inline void pop_int64_t(int64_t& out, bool& error)
{
	error = error || pop_int64_t(out);
}
inline void pop_float(float& out, bool& error)
{
	error = error || pop_float(out);
}
inline void pop_double(double& out, bool& error)
{
	error = error || pop_double(out);
}

//--------------------------------------------------------------------------------------------------------------------------------

// T pop_T(bool& error);
inline uint8_t pop_uint8_t(bool& error)
{
	uint8_t r = uint8_t(0);
	error = error || pop_uint8_t(r);
	return r;
}

inline uint16_t pop_uint16_t(bool& error)
{
	uint16_t r = uint16_t(0);
	error = error || pop_uint16_t(r);
	return r;
}

inline uint32_t pop_uint32_t(bool& error)
{
	uint32_t r = uint32_t(0);
	error = error || pop_uint32_t(r);
	return r;
}

inline uint64_t pop_uint64_t(bool& error)
{
	uint64_t r = uint64_t(0);
	error = error || pop_uint64_t(r);
	return r;
}

inline int8_t pop_int8_t(bool& error)
{
	int8_t r = int8_t(0);
	error = error || pop_int8_t(r);
	return r;
}

inline int16_t pop_int16_t(bool& error)
{
	int16_t r = int16_t(0);
	error = error || pop_int16_t(r);
	return r;
}

inline int32_t pop_int32_t(bool& error)
{
	int32_t r = int32_t(0);
	error = error || pop_int32_t(r);
	return r;
}

inline int64_t pop_int64_t(bool& error)
{
	int64_t r = int64_t(0);
	error = error || pop_int64_t(r);
	return r;
}

inline float pop_float(bool& error)
{
	float r = float(0);
	error = error || pop_float(r);
	return r;
}

inline double pop_double(bool& error)
{
	double r = double(0);
	error = error || pop_double(r);
	return r;
}


//--------------------------------------------------------------------------------------------------------------------------------
