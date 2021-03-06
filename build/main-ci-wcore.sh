#!/bin/bash
set -e -o pipefail

MAIN_WORKSPACE=$1
THIS_WORKSPACE=$2

python3 ${THIS_WORKSPACE}/cllio/prj/prj.cllio-test.py -a make linux

#build and stuff
cd ${THIS_WORKSPACE}/cllio/prj/build/cllio-test_linux_make
make config=debug_x64 all
make config=release_x64 all

#run leak detection
valgrind --leak-check=full --log-fd=1 --error-exitcode=1911 ./bin/x64/Debug/_cllio-test

python3 -c "import ciutil; ciutil.files_equal('samples.bin','../../../tests/samples.bin')"

#run perf test
./bin/x64/Release/_cllio-test

