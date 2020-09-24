#!/bin/bash

python3 /wcore/workspace/cllio/prj.cllio-test.py make linux

#build and stuff
cd /wcore/workspace/cllio/build/cllio-test_linux_make
make config=debug_x32 all

./bin/x32/Debug/_cllio-test

python3 -c "import ciutil; ciutil.files_equal('samples.bin','../../tests/samples.bin')"