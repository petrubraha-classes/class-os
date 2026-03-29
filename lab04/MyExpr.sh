#!/bin/bash

file=""
if [ $# -eq 1 ]
then
  file=$1
fi

let count_operations=0

echo "mota" > $file

until 
  read -p "read first operand" nr1
  read -p "read secnd operand" nr2
  read -p 