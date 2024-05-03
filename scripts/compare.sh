#!/bin/bash

python3 ../scripts/compare.py ../reference/ref_100.txt ../results/res_100.txt > ../speed_up/speed_up_100.txt
python3 ../scripts/compare.py ../reference/ref_500.txt ../results/res_500.txt > ../speed_up/speed_up_500.txt
python3 ../scripts/compare.py ../reference/ref_1000.txt ../results/res_1000.txt > ../speed_up/speed_up_1000.txt