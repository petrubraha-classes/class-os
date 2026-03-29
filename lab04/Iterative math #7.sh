#!/bin/bash

# argument validation
declare -i n=0 k=0
if [ $# -gt 2 ]; then
  echo "error: wrong number of arguments"
  exit 1
elif [ $# -eq 2 ]; then
  n=$1
  k=$2
else
  read -p "insert n: " n
  read -p "insert k: " k
fi

# input validation
if [ $n -lt 0 ]; then
  echo "error: invalid parameter, provide a positive value"
  exit 1
elif [ $k -lt 0 ]; then
  echo "error: invalid parameter, provide a positive value"
  exit 1
elif [ $k -gt $n ]; then
  echo "error: k must be smaller of equal to n"
  exit 1
fi

function factorial(){
  declare -i product=1 variable=$1
  for i in $(seq 1 $n); do
    product=${product}*$variable
    variable=$variable-1
  done
}

declare -i up=${factorial $n}, down=${factorial $($n - $k)}
result=((up/down))
echo $result
