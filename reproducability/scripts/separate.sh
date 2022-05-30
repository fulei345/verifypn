#!/bin/bash
for i in 1 2 3 4 ; do
    cp -r app$i app$i-ft
    rm -rf app$i/\false
    rm -rf app$i/\true
    rm app$i-ft/*results
done
