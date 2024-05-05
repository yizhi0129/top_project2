#!/bin/bash
mkdir -p ../results
mkdir -p ../speed_up

cd ../src

cmake .
make

j=24
perf record ./my_executable ../config.txt ../results/res_100_-O3_$j.txt
perf report > ../speed_up/perf_100_-O3_$j.txt

perf record ./my_executable ../config_500.txt ../results/res_500_-O3_$j.txt
perf report > ../speed_up/perf_500_-O3_$j.txt

perf record ./my_executable ../config_1000.txt ../results/res_1000_-O3_$j.txt
perf report > ../speed_up/perf_1000_-O3_$j.txt


cd ../scripts
./clean.sh