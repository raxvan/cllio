#!/bin/bash
set -e -o pipefail

MAIN_WORKSPACE=$1
THIS_WORKSPACE=$2

python3 ${THIS_WORKSPACE}/cllio/prj.cllio-test.py -a make linux

#build and stuff
cd ${THIS_WORKSPACE}/cllio/build/cllio-test_linux_make
make config=debug_x64 all

#run leak detection
valgrind --leak-check=full --log-fd=1 --error-exitcode=1911 ./bin/x64/Debug/_cllio-test

python3 -c "import ciutil; ciutil.files_equal('samples.bin','../../tests/samples.bin')"


