#!/bin/bash
BIN="verifypn-linux64-eval"
F=MCC2021

for i in $(ls $F) ; do
    for m in ALL AM A ; do
        for q in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do
            sbatch --job-name="$m-$i-$q" -n 1 ./run_engine_2.sh $BIN $m $i $q
        done
    done
done
