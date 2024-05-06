#!/bin/bash

for i in 1 2 4 8 16 24 48
do
python3 ../scripts/compare.py ../reference/ref_100.txt ../results/res_100_o_$i.txt > ../speed_up/scal_100_o_$i.txt
python3 ../scripts/compare.py ../reference/ref_500.txt ../results/res_500_o_$i.txt > ../speed_up/scal_500_o_$i.txt
python3 ../scripts/compare.py ../reference/ref_1000.txt ../results/res_1000_o_$i.txt > ../speed_up/scal_1000_o_$i.txt
done