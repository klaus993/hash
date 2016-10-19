#!/bin/bash

CFLAGS='-g -std=c99 -Wall -Wconversion -Wno-sign-conversion'

gcc $CFLAGS -c -o main.o main.c
gcc $CFLAGS -c -o pruebas_catedra.o pruebas_catedra.c
gcc $CFLAGS -c -o hash.o hash.c
gcc $CFLAGS -c -o testing.o testing.c
gcc $CFLAGS -c -o f_hash.o f_hash.c
gcc $CFLAGS -o pruebas.out main.o pruebas_catedra.o hash.o f_hash.o testing.o
