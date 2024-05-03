#!/bin/bash
mkdir -p ../results
mkdir -p ../speed_up

cd ../src

cmake .
make

perf record ./my_executable ../config.txt ../results/res_100.txt
perf report > ../speed_up/perf_100.txt

perf record ./my_executable ../config_500.txt ../results/res_500.txt
perf report > ../speed_up/perf_500.txt

perf record ./my_executable ../config_1000.txt ../results/res_1000.txt
perf report > ../speed_up/perf_1000.txt

cd ../scripts
./clean.sh