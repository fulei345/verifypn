#!/bin/bash
for m in 1 2 3 4 ; do
    mkdir app$m-const
    cd app$m-const
    mkdir \true
    cp ../../heuristicsresults/app$m-true-results \true/
    for t in bfs dfs heur ; do
        cat ../../tapaalresults/$t\negativeresults >> \true/negativeresults
    done
    python3 ../main.py --f \true/ -format 3 --i

    mkdir \false
    cp ../../heuristicsresults/app$m-false-results \false/
    for t in bfs dfs heur ; do
        cat ../../tapaalresults/$t\positiveresults >> \false/positiveresults
    done
    python3 ../main.py --f \false/ -format 3 --i
    cd ..
done
