#!/bin/bash

# argument validation
declare -i n=0
if [ $# -gt 1 ]; then
  echo "error: wrong number of arguments"
  exit 1
elif [ $# -eq 1 ]; then
  n=$1
else
  read -p "insert n: " n
fi

# input validation
if [ $n -lt 0 ]; then
  echo "error: invalid parameter, provide a positive value"
  exit 1
elif [ $n -eq 0 ]; then
  echo "sum = 0"
  exit 0
elif [ $n -eq 1 ]; then
  echo "sum = 1"
  exit 0
elif [ $n -eq 2 ]; then
  echo "sum = 1"
  exit 0
fi

# loop
declare -i n0=1 n1=1 temp=0
for i in $(seq 3 $n); do
  temp=$n0
  n0=$n1
  n1=$n0+$temp
done

echo "sum = $n1"
