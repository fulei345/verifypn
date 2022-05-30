#!/bin/bash
for m in 1 2 3 4 ; do
    for f in 'true' 'false' ; do
        grep -H \grep,$f ./output-app$m/* > ./heuristicsresults/app$m-$f-results
    done
done
