#!/bin/bash
set -e

MAIN_WORKSPACE=$1
THIS_WORKSPACE=$2

/bin/bash $MAIN_WORKSPACE/exec.sh $THIS_WORKSPACE python3 /wcore/workspace/cllio/prj.cllio-test.py -a make linux

#build
cd ${THIS_WORKSPACE}/cllio/build/cllio-test_linux_make
make config=debug_arm all
make config=release_arm all

#run exe
./bin/ARM/Debug/_cllio-test

#compare files
/bin/bash $MAIN_WORKSPACE/exec.sh $THIS_WORKSPACE cmp /wcore/workspace/cllio/build/cllio-test_linux_make/samples.bin /wcore/workspace/cllio/tests/samples.bin


#run perf test
./bin/ARM/Release/_cllio-test