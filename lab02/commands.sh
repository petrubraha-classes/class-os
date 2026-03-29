#!/bin/bash

#mandatory
wc -l /etc/mtab ; wc -L /etc/mtab

find $HOME -type f -mtime 14 -exec stat -c %A,%n,%y {} \;

find . -type f -perm -u=wx -exec stat -c %n,%x,%s {} \;

#extra
stat ../lab\ 1\  done/program1.c ../lab\ 1\ done/program2.c ../lab\ 1\ done/program2.h

chmod u-r  ../lab\ 1\ done/program1.c
chmod ug+x ../lab\ 1\ done/program2.c
chmod a=   ../lab] 1\ done/program2.h ; chmod  ug=r ../lab\ 1\ done/program2.h

sort -t: -k3,3nr -r /etc/group

# find -type f,d -group $(stat -c %G #path#) -perm -g=r,x -exec stat -c %n,%A {} \;

find / -type f -name "*.c" -o -name "*.h" -o -name "*.cpp" -exec stat -c %n,%s,%y {} \;

find /home -name "*.cpp" -exec if [ $(du  -k) < 5 ]; then g++ 2>errors.txt; fi {} \;
