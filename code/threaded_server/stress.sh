#!/bin/sh

# This script stresses the serveur

# The host on which the serveur is running
HOST=localhost
# The port the server is serving to
PORT=8080
# The interval, in seconds, between http requests
INTERVAL=0.01
# The file to get. Empty string means root, and often
# defaults to index.html or whatever.
FILE="/ubuntu-10.10-desktop-i386.iso"
# How many connection ? 0 means unlimited
CONNECTIONS=100

stress()
{
  wget $HOST:$PORT$FILE -O /dev/null -o /dev/null && echo "$1 : Done" &
  sleep $INTERVAL
}



if [ $CONNECTIONS -ne 0 ]
then
  for i in $(seq $CONNECTIONS)
  do
    stress $i
  done
else
  i=0
  while : 
  do
    stress
    i=$($i+1)
  done
fi
