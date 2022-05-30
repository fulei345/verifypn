#!/bin/bash
#SBATCH --time=00:05:00
#SBATCH --partition=dhabi
#SBATCH --mem=15000
#SBATCH --output=/nfs/home/student.aau.dk/tbsh18/MCC/output-app1/%x
export VERIFYPN=$(pwd)/binaries/$1
export MODEL_PATH=$(pwd)/MCC2021/$2

$VERIFYPN -smc 1000000 10000 -i A -u 1 -x $3 $MODEL_PATH/model.pnml $MODEL_PATH/ReachabilityCardinality.xml

