#!/bin/bash
echo usage:
echo $0 servers_num threads_num


touch servers.txt
I=0
cat /dev/null > servers.txt
chmod 777 servers.txt
while [[ $I -lt $1 ]]
do
./server --port $[20001+$I] --tnum $2 &
echo 127.0.0.1:$[20001+$I] >> servers.txt
echo started server $I
((I++))
done
