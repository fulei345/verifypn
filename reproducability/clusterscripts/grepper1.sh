#!/bin/bash
for m in smc smcn ; do
    grep -H ,1000000 ./output-exp1/$m-* > ./exp1results/"$m"output
done
