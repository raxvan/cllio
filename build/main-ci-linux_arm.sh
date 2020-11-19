#!/bin/bash
set -e

MAIN_WORKSPACE=$1
THIS_WORKSPACE=$2

/bin/bash $MAIN_WORKSPACE/exec.sh $THIS_WORKSPACE python3 /wcore/workspace/cllio/prj.cllio-test.py -a cmake linux

#build
cd ${THIS_WORKSPACE}/cllio/build/cllio-test_linux_cmake
cmake .
cmake --build . --config Debug

#run exe
./bin/Debug/_cllio-test

#compare files
/bin/bash $MAIN_WORKSPACE/exec.sh $THIS_WORKSPACE cmp /wcore/workspace/cllio/build/cllio-test_linux_make/samples.bin /wcore/workspace/cllio/tests/samples.bin



