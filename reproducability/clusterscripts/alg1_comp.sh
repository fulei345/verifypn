#!/bin/bash
BIN="verifypn-linux64-non"
F=MCC2021

for i in $(ls $F) ; do
    for m in smc smcn ; do
        sbatch --job-name="$m-$i" -n 1 ./run_engine_1.sh $BIN $m $i
    done
done
