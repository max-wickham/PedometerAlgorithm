#! /bin/sh
python3 createC.py
g++ output.c -o output.o
./output.o
rm output.o