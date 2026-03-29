#!/bin/bash

if [ $# -ne 1 ]; then
	echo "error: missing <directory>"
	exit 1
fi

if [ ! -d $1 ]; then
	echo "error: $1 doesn't exist" >&2
	exit 1
fi

if [ ! -r $1 ] || [ ! -x $1 ]; then
	echo "error: $1 has not read/execution rights" >&2
	exit 2
fi

./subdir/explain.sh $1
