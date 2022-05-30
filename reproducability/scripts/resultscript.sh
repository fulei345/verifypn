#!/bin/bash
for p in 1 2 3 4 ; do
    mkdir app$p
    for m in 'true' 'false' ; do
	mkdir app$p/$m
        cp ../heuristicsresults/app$p-$m-results app$p/$m
    done
    cp ../tapaalresults/*negative* app$p/\false
    cp ../tapaalresults/*positive* app$p/\true
    for i in dfs bfs heur app$p ; do
        cat app$p/*/$i* >> app$p/$i-results
    done
    cat ../tapaalresults/notime* >> app$p/notime-results
done

