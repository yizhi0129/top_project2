#!/bin/bash
mkdir -p ../results
mkdir -p ../speed_up

cd ../src

cmake .
make

j=1
perf record ./my_executable ../config.txt ../results/res_100__Ofast_$j.txt
perf report > ../speed_up/perf_100__Ofast_$j.txt

perf record ./my_executable ../config_500.txt ../results/res_500__Ofast_$j.txt
perf report > ../speed_up/perf_500__Ofast_$j.txt

perf record ./my_executable ../config_1000.txt ../results/res_1000__Ofast_$j.txt
perf report > ../speed_up/perf_1000__Ofast_$j.txt


cd ../scripts
./clean.sh
