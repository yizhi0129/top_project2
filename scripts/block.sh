#!/bin/bash
mkdir -p ../results
mkdir -p ../speed_up

cd ../src

cmake .
make

k=64
perf record ./my_executable ../config_500.txt ../results/res_500_block$k.txt
perf report > ../speed_up/perf_500_block$k.txt

cd ../scripts
./clean.sh
