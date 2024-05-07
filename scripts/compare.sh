#!/bin/bash

i=24
python3 ../scripts/compare.py ../reference/ref_100.txt ../results/res_100_ijk_$i.txt > ../speed_up/sp_100_ijk_$i.txt
python3 ../scripts/compare.py ../reference/ref_500.txt ../results/res_500_ijk_$i.txt > ../speed_up/sp_500_ijk_$i.txt
python3 ../scripts/compare.py ../reference/ref_1000.txt ../results/res_1000_ijk_$i.txt > ../speed_up/sp_1000_ijk_$i.txt
