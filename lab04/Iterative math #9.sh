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
if [ $n -lt 1 ]; then
  echo "error: invalid parameter, provide n > 0"
  exit 1
fi

# loop
declare -i product=1 variable=$n
for i in $(seq 1 $n); do
  product=${product}*$variable
  variable=$variable-1
done

echo "n! = $product"
