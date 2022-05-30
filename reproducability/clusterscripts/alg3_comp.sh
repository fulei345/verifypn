#!/bin/bash
BIN="verifypn-linux64-search"
F=MCC2021

for m in 1 2 3 4; do
    for i in $(ls $F) ; do
        for q in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do
            sbatch --job-name="$i-$q" -n 1 ./run_engine_3_$m.sh $BIN $i $q
        done
    done
done
