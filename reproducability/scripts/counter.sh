#!/bin/bash
for i in 1 2 3 4 ; do
    echo ~app $i:
    for m in bfs dfs heur app$i ; do
        echo $m:
	echo total: $(cat app$i/$m-results | wc -l)
	for f in -merged -solo ; do
            csvcount=$(cat app$i/$m-$f.csv | wc -l)
            fixed=$(($csvcount-1))
	    echo $f: $fixed
        done
    done
    echo -----
done
