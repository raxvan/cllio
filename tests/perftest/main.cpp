
#include <cllio.h>
#include <limits>
#include <iostream>
#include <vector>
#include <cstring>
#include <functional>
#include "disk_speed_test.h"

int main(int argc, const char** argv)
{

	// run test only on release
	if (argc > 1)
	{
		disk_access_test speed_test; // run speed test
		const char*		 file = argv[1];
		if (speed_test.run(file) == false)
			return -1;
	}
	else
	{
		disk_access_test speed_test; // run speed test
		if (speed_test.run(nullptr) == false)
			return -1;
	}

	return 0;
}
