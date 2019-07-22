#!/bin/bash -xe

python3 prj.cllio_tests.py make linux
cd build/cllio_tests_linux_make

make config=debug_arm

./bin/ARM/Debug/_cllio_tests
python3 -c "import civalidate; civalidate.files_equal('samples.bin','../../tests/samples.bin')"

