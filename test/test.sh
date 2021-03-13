#!/usr/bin/env bash

python3 createCtest.py
gcc output.c -o output.o
./output.o > output.txt
python3 plottest.py
