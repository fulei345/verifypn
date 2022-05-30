#!/bin/bash
for m in bfs dfs heur app1 app2 app3 app4;  do
    echo ~$m:
    echo timeouts:
    grep -H 'could not be encoded' output-$m/* | wc -l
    grep -H slurm output-$m/* | wc -l
    echo solved:
    grep -H 'Query is' output-$m/* | wc -l
    echo using methods:
    grep -H 'time,' output-$m/* | wc -l
    echo ----------
done
