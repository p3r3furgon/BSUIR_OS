#!/bin/bash
echo 'int main(){printf("HELLO Ubuntu\n");}' > 1.c
gcc -include stdio.h 1.c -o 1.exe
./1.exe
./del3.sh
