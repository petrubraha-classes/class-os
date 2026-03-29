#!/bin/bash

pow(){
  if [ $# -ne 2 ]
  then 
    echo "error: function - wrong number of arguments"
    exit 2
  fi

  local base=$1
  local exponent=$(($2-1))

  if [ $2 -eq 0 ]; then
    echo 1
  elif [ $2 -eq 1 ]; then
    echo $base
  else
    echo $(($base * $(pow $base $exponent)))
  fi
}

declare -i k=0
if [ $# -ge 2 ]
then
  echo "error: wrong number of arguments"
  exit 1
elif [ $# -eq 0 ]
then
  read -p "type an integer: " k
else
  k=$1
fi

declare -i base=2
for i in $(seq 0 $k); do
  echo $(pow $base $i)
done
