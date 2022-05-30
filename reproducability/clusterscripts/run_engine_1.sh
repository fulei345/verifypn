#!/bin/bash
#SBATCH --time=00:15:00
#SBATCH --partition=dhabi
#SBATCH --mem=15000
#SBATCH --output=/nfs/home/student.aau.dk/tbsh18/MCC/output-exp1/%x
export VERIFYPN=$(pwd)/binaries/$1
export MODEL_PATH=$(pwd)/MCC2021/$3

$VERIFYPN -$2 5 1000000 -x 1 $MODEL_PATH/model.pnml $MODEL_PATH/ReachabilityCardinality.xml
