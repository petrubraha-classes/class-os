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

function is_prime()
{
  if [ $# -ne 1 ]; then
  	echo "error - is_prime(): wrong number of arguments"
    exit 2
  fi

  if [ $1 -eq 2 ] || [ $1 -eq 3 ] || [ $1 -eq 5 ] || [ $1 -eq 7 ]; then
    echo "1"
  else
    echo "0"
  fi
}

function sum()
{
  if [ $# -ne 1 ]; then
  	echo "error - sum(): wrong number of arguments"
    exit 2
  fi

  if [ $1 -eq 0 ]; then
    echo "0"
  else
    local next_digit=$(($1/10))
    local last_digit=$(($1%10))
    local result=$(is_prime $last_digit)
    if [ $result -eq 1 ]; then
      echo $((last_digit + $(sum $next_digit)))
    else
      echo $(sum $next_digit)
    fi
  fi
}

echo $(sum $n)
