cut -d: -f1,3 /etc/group

cut -d: -f3 /etc/passwd | grep ':0' | id

stat -c %h lmao_link # nr of hard links
