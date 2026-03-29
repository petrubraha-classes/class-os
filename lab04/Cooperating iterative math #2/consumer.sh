#!/bin/bash

declare -a arr
read -a arr

declare -i sum=0 cube=2
for element in ${arr[@]}; do
  cube=$(($element ** 3))
  sum+=$cube
done

if [ $cube -eq 2 ]; then
  result="NULL"
else
  result=$sum
fi

echo "Consumer's computed sum is: $result"
if [ ${#arr[@]} -eq 0 ]; then
  exit 1
fi
exit 0
