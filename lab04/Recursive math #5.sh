#!/bin/bash

declare -i n=0
if [ $# -gt 1 ]; then
	echo "error: too many arguments"
	exit 1
elif [ $# -eq 1 ]; then
  n=$1
else
  read -p "insert n: " n
fi

if [ 0 -ge $n ]; then
	echo "error: invalid value"
	exit 1
fi

function custom_flip()
{
  if [ $# -ne 1 ]; then
    echo "errror - custom_flip(): invalid number of arguments"
    exit 2
  fi

  if [ $1 -lt 10 ]; then
    echo $((9 - $1))
  else
    local digit=$((9 - $1%10))
    echo -n $digit
    custom_flip $(($1/10))
  fi
}

echo $(custom_flip $n)
