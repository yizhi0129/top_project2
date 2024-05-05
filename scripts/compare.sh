#!/bin/bash

python3 ../scripts/compare.py ../reference/ref_100.txt ../results/res_100_-O3_24.txt > ../speed_up/speed_up_100_-O3.txt
python3 ../scripts/compare.py ../reference/ref_500.txt ../results/res_500_-O3_24.txt > ../speed_up/speed_up_500_-O3.txt
python3 ../scripts/compare.py ../reference/ref_1000.txt ../results/res_1000_-O3_24.txt > ../speed_up/speed_up_1000_-O3.txt

python3 ../scripts/compare.py ../reference/ref_100.txt ../results/res_100_-Ofast_24.txt > ../speed_up/speed_up_100_-Ofast.txt
python3 ../scripts/compare.py ../reference/ref_500.txt ../results/res_500_-Ofast_24.txt > ../speed_up/speed_up_500_-Ofast.txt
python3 ../scripts/compare.py ../reference/ref_1000.txt ../results/res_1000_-Ofast_24.txt > ../speed_up/speed_up_1000_-Ofast.txt
