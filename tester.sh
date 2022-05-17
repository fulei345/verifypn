#!/bin/bash
x=$1; shift;
echo "Model: $x"
for q in $@ ; do
    echo "----------------------------------------------"
    for m in ALL A AM ; do
	echo "query: $q, $m"
        for i in 1 2 3 4 5; do
            timeout 5m ./verifypn-linux64 -q 0 -smc 10000 10000 -i $m -x $q ../../../select_models/$x/model.pnml ../../../select_models/$x/ReachabilityCardinality.xml | grep grep
        done
    done
done