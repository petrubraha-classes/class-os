#!/bin/bash

declare -a arr
arr=$@

if [ $# -eq 0 ]; then
  read -p "insert numbers: " -a arr
fi

declare -i number_bound
read -p "insert p: " number_bound

for element in ${arr[@]}; do
  if [ $element -ge $number_bound ]; then
    echo -n "$element "
  fi
done
