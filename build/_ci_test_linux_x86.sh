#!/bin/bash -xe

python3 prj.cllio_tests.py make linux
cd build/cllio_tests_linux_make

make config=debug_x32
make config=debug_x64

./bin/x32/Debug/_cllio_tests
python3 -c "import civalidate; civalidate.files_equal('samples.bin','../../tests/samples.bin')"

./bin/x64/Debug/_cllio_tests
python3 -c "import civalidate; civalidate.files_equal('samples.bin','../../tests/samples.bin')"


