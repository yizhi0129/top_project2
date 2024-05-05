#!/bin/bash

for i in 40 48 56
do
python3 ../scripts/compare.py ../reference/ref_500.txt ../results/res_500_block$i.txt > ../speed_up/speed_up_500_block$i.txt
done