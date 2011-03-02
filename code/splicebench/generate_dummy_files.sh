#! /bin/sh -x

# This script generates data files, to be transfered to bench

(cat /dev/urandom > file1) &

PID=$!

sleep 4

kill $PID

cp file1 file1rw
