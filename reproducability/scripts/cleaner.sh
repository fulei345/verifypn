#!/bin/bash
for i in 1 2 3 4 ; do
    python3 main.py --f app$i/ -format 3 --s
    mv app$i/notime-results .
    python3 main.py --f app$i/ -format 3 --i --w
    mv notime-results app$i/
    python3 main.py --f app$i-ft/\true -format 3 --i --w
    python3 main.py --f app$i-ft/\true -format 3 --s
done
