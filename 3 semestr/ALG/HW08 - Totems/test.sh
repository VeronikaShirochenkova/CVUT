#!/bin/bash

for i in "01" "02" "03" "04" "05" "06" "07" "08" "09" "10"
do
    str="datapub/pub${i}.in"
    ./hw08 <$str
    echo "----"
done