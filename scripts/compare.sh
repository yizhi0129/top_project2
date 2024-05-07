#!/bin/bash

for i in 2 4 8 16 24 48
do
python3 ../scripts/compare.py ../results/res_100_o_1.txt ../results/res_100_o_$i.txt > ../speed_up/sp_100_o_$i.txt
python3 ../scripts/compare.py ../results/res_500_o_1.txt ../results/res_500_o_$i.txt > ../speed_up/sp_500_o_$i.txt
python3 ../scripts/compare.py ../results/res_1000_o_1.txt ../results/res_1000_o_$i.txt > ../speed_up/sp_1000_o_$i.txt
done