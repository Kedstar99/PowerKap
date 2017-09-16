#include !/bin/bash
mkdir pstate
profKap -c -e -t 300 -- /usr/bin/stress -c 1
mkdir cpu1
mv *.json cpu1
mv cpu1 pstate
profKap -c -e -t 300 -s -- /usr/bin/stress -c 2
mkdir cpu2
mv *.json cpu2
mv cpu2 pstate
profKap -c -e -t 300 -s -- /usr/bin/stress -c 3
mkdir cpu3
mv *.json cpu3
mv cpu3 pstate
profKap -c -e -t 300 -s -- /usr/bin/stress -c 4
mkdir cpu4
mv *.json cpu4
mv cpu4 pstate
