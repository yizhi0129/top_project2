#!/bin/bash

python3 ../scripts/compare.py ../reference/ref_100.txt ../results/res_100_24.txt > ../speed_up/speed_up_100_ind.txt
python3 ../scripts/compare.py ../reference/ref_500.txt ../results/res_500_24.txt > ../speed_up/speed_up_500_ind.txt
python3 ../scripts/compare.py ../reference/ref_1000.txt ../results/res_1000_24.txt > ../speed_up/speed_up_1000_ind.txt