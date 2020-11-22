
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

class disk_access_test
{
public:
	bool run(const char* path_to_test_file);
	bool run_internal(const char* path_to_test_file, const bool validate_results);

	bool generate_sample_file(const char* path);
};