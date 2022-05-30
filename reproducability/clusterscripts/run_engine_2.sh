#!/bin/bash
#SBATCH --time=00:05:00
#SBATCH --partition=dhabi
#SBATCH --mem=15000
#SBATCH --output=/nfs/home/student.aau.dk/tbsh18/MCC/output-exp2/%x
export VERIFYPN=$(pwd)/binaries/$1
export MODEL_PATH=$(pwd)/MCC2021/$3

$VERIFYPN -q 0 -smc 10000 10000 -i $2 -x $4 $MODEL_PATH/model.pnml $MODEL_PATH/ReachabilityCardinality.xml
