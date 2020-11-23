
#include <cllio.h>
#include <limits>
#include <iostream>
#include <vector>
#include <cstring>
#include <functional>

struct profiler_timer
{
	const char*									   name;
	std::chrono::high_resolution_clock::time_point start_time;

	profiler_timer(const char* _name);
	~profiler_timer();
};

struct read_validator
{
	uint32_t refvalue = 0;
	bool	 initialized = false;

	bool validate(const uint32_t value, const char* reader);
};

class disk_access_test
{
public:
	bool run(const char* path_to_test_file);
	bool run_internal(const char* path_to_test_file, const bool validate_results, const std::size_t known_content_size);

	std::size_t generate_sample_file(const char* path);
};