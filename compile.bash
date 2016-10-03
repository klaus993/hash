#!/bin/bash

gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -c -o main.o main.c
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -c -o pruebas_catedra.o pruebas_catedra.c
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -c -o hash.o hash.c
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -c -o testing.o testing.c
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -c -o f_hash.o f_hash.c
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -o pruebas.out main.o pruebas_catedra.o hash.o f_hash.o testing.o
