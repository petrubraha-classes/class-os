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
fi

command="find $HOME -maxdepth $n -type f -mtime -28"
files=$($command)

if [ $? -ne 0 ]; then
	echo "error: command find failed" >&2
	exit 1
fi

for file in $files; do

	echo -n "$file - "
	file $file | grep -q "C source"

	if [ $? -eq 0 ]; then
		d=$(dirname $file)
		b=$(basename $file)
		gcc $file -o "$d/${b%.*}"
		if [ $? -eq 0 ]; then
			echo "compilation ok"
		else
			echo "compilation error"
		fi

	else # normal file
		echo stat -c %y $file
	fi
done
