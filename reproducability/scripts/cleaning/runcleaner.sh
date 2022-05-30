#!/bin/bash
for l in $(ls *) ; do
    python3 cleaner.py $l  
done
