#!bin/bash

# ex1
mkdir ~/programe

mkdir ~/programe/tema1 ~/programe/tema2

mkdir ~/programe/tema2/sub-temaA

mv ~/programe/tema2/sub-temaA ~/programe/tema2/tema2_sub-temaA

#ex2
touch ~/programe/program1.c ~/programe/program2.c ~/programe/program2.h

touch ~/programe/tema1/tema1-1.c ~/programe/tema1/tema1-2.c

touch ~/programe/tema2/tema2_sub-temaA/sub-temaA1.c 
touch ~/programe/tema2/tema2_sub-temaA/sub-temaA2.cpp

# iv)
ln ~/programe/tema2/tema2_sub-temaA/sub-temaA1.c ~/programe/tema2/tema2_sub-temaA/tema2_sub-temaA1.c
ln -s ~/programe/tema2/tema2_sub-temaA/sub-temaA2.cpp ~/programe/tema2/tema2_sub-temaA/tema2_sub-temaA2.cpp

# v)
cp ~/programe/tema1/tema1-1.c ~/programe/tema2/tema2-1.c

# vi)
mv ~/programe/tema1/tema1-2.c ~/programe/tema2/tema2-2.c

# vii)
rm ~/programe/tema2/tema2_sub-temaA/tema2_sub-temaA1.c

#ex3
cut -d: -f1,3 --output-delimiter="-" /etc/group

#ex4
grep ":0:" /etc/passwdq

#ex5
stat -c %h ~/Documents/WhatsApp\ Image\ 2025-02-01\ at\ 22.45.52.jpeg
