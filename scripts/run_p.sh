#!/bin/bash
mkdir -p ../results
mkdir -p ../speed_up

cd ../src

cmake .
make

for i in 1 2 4 8 16 32 48
do
    perf record mpirun -np $i ./my_executable ../config.txt ../results/res_100_$i.txt
    perf report > ../speed_up/perf_100_$i.txt

    perf record mpirun -np $i ./my_executable ../config_500.txt ../results/res_500_$i.txt
    perf report > ../speed_up/perf_500_$i.txt

    perf record mpirun -np $i ./my_executable ../config_1000.txt ../results/res_1000_$i.txt
    perf report > ../speed_up/perf_1000_$i.txt
done

cd ../scripts
./clean.sh