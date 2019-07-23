#!/bin/bash -xe

python3 prj.cllio_test.py make linux
cd build/cllio_test_linux_make

make config=debug_arm

./bin/ARM/Debug/_cllio_test
python3 -c "import civalidate; civalidate.files_equal('samples.bin','../../tests/samples.bin')"

