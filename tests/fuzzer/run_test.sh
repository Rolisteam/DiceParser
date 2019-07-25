#!/bin/sh

rm -rf build
mkdir build
cd build
CC=afl-gcc CXX=afl-g++ cmake ../ -DSTATIC_BUILD=ON && make
#export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"


#./fuzzTestCase ../testcase_dir/gooddata.txt
# afl-fuzz -m 2G -i ../testcase_dir -o ../findings_dir ./fuzzTestCase @@
afl-fuzz -m 2G -i ../testcase_dir -o ../sync_dir -M fuzzer01 ./fuzzTestCase @@
afl-fuzz -m 2G -i ../testcase_dir -o ../sync_dir -S fuzzer02 ./fuzzTestCase @@
afl-fuzz -m 2G -i ../testcase_dir -o ../sync_dir -S fuzzer03 ./fuzzTestCase @@
afl-fuzz -m 2G -i ../testcase_dir -o ../sync_dir -S fuzzer04 ./fuzzTestCase @@
afl-fuzz -m 2G -i ../testcase_dir -o ../sync_dir -S fuzzer05 ./fuzzTestCase @@
afl-fuzz -m 2G -i ../testcase_dir -o ../sync_dir -S fuzzer06 ./fuzzTestCase @@



# as root

# echo core >/proc/sys/kernel/core_pattern

# echo performance | tee cpu*/cpufreq/scaling_governor

# echo  ondemand | tee cpu*/cpufreq/scaling_governor
