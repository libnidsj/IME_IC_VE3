#!bin/bash

rm output/*

gcc -std=c99 continencia.c -o output/continencia.exe
gcc -std=c99 professor.c -o output/professor.exe
gcc -std=c99 ten_sugao.c -o output/ten_sugao.exe