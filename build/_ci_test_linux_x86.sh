#!/bin/bash -xe

python3 prj.cllio_test.py make linux
cd build/cllio_test_linux_make

make config=debug_x32
make config=debug_x64

./bin/x32/Debug/_cllio_test
python3 -c "import ciutil; ciutil.files_equal('samples.bin','../../tests/samples.bin')"

./bin/x64/Debug/_cllio_test
python3 -c "import ciutil; ciutil.files_equal('samples.bin','../../tests/samples.bin')"


