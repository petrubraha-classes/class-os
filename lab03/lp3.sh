# sort -k3 coloana 3
# tr -s schimba caractere

#!/bin/sh 

#1)
# deschizi file
# usernames care se termina cu o litera intre m si z
# care folosesc ca și interpretor de login programul /sbin/nologin
# primii 15
# redirect
cut -d : -f 1,7 /etc/passwd | grep "/sbin/nologin" | grep -E "[.]*[m-z]:" | head -n 15 > max15.txt

head -n 15 /etc/passwd | cut -d : -f1,7 | grep "/sbin/nologin" | grep -E "[.]*[m-z]" > max15.txt

#2)
# user - bash,
# terminale desemnate prin ID-uri (i.e., pts/XY) 
# ce încep cu cifrele 0, 1, 2 sau 5, 
# sort -r pid
# write procese
# redirect
ps -axo user,cmd,pid,tty | grep -e ":pts/[0|1|2|5][.]*" | grep ":bash:" | sort -t: -r -nk3 > output.txt

#3)
last -i | head -n 8 | tr -s " " | cut -d" " -f3 > filtruIPs.txt

#4)
ps -eo user,comm,pid --no-headers | tr -s " " | cut -d" " -f1,2,3 --output-delimiter=: | sort -k2 -t:
#ps -eo user,comm,pid --no-headers | tr -s " " ":" | sort -k2 -t:

#1)
#? Să se scrie comanda înlănțuită care afișează datele: numele de utilizator, UID-ul și GID-ul, în formatul "username -> UID -> GID", pentru toți utilizatorii sistemului, sortați crescător după UIDq

getent passwd | awk -F: '{print $1 " -> " $3 " -> " $4}' | sort -t">" -k2 -n

#2)
#? Să se scrie comanda înlănțuită care afișează ultimii 15 utilizatori conectați la sistem, în funcție de ora conectării, precum și data, ora și stația de la care s-au conectat. (Indicație: procesați outputul comenzii last.)

last -n 15 | tr -s " " | awk '{print $1, $4, $5, $6, $7, $2}'

#3)
#? Să se scrie comanda înlănțuită care afișează, pentru fiecare utilizator conectat la sistem, următoarele informații: numele de cont, stația de la care s-a conectat și procesul rulat în foreground, ordonate în ordinea alfabetică a numelor de cont. (Indicație: folosiți înlănțuiri de comenzi simple, prelucrând informațiile despre utilizatorii conectați la sistem în acel moment.)

who | tr -s " " | cut -d" " -f1,2 | sort -k1 | ps -u $1 -t $2
#not correct

#4)
#? Să se scrie comanda înlănțuită care afișează toți utilizatorii sistemului care au UID-ul mai mare (strict) decât 4681.(Indicație: folosiți înlănțuiri de comenzi simple și informațiile din fișierul /etc/passwd. Pentru a descrie numere mai mari sau egale cu 4682, se poate folosi comanda grep cu un șablon de tip expresie regulată.)

awk -F: '$3 > 4681' /etc/passwd
awk -F: '$3 > 4681{print $1, $3}' /etc/passwd
