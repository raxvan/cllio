python3 prj.cllio_tests.py make linux
cd build/cllio_tests_linux_make

make config=debug_x32
make config=debug_x64

cd cllio_tests_linux_make
./bin/x32/Debug/_cllio_tests
cd ..