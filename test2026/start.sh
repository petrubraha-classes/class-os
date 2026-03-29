#!/bin/bash

# recommended: ./start.sh 1 1

# compilation
rm -f fifo2025
gcc supervisor.c -o supervisor
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

gcc worker1.c -o worker1
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

gcc worker2+3.c -o worker2+3 -lm
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

# parameters
if [ $# -ne 2 ]; then
  echo "error: wrong number of arguments"
  exit 1
fi

N=$1
M=$2
file="input_data.txt"

{ sleep $N ; ./supervisor $file ; } & sleep $M ; ./worker2+3 ;

# delete remainders
echo
echo "the content of the map"
od -t d4 -An /dev/shm/mem_shrd | tr -s " "

rm -f /dev/shm/mem_shrd
rm -f fifo2025
rm supervisor worker1 worker2+3