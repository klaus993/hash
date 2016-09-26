gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -c -o main.o main.c
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -c -o pruebas_catedra.o pruebas_catedra.c
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -c -o hash.o hash.c
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -c -o testing.o testing.c
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -o pruebas.out main.o pruebas_catedra.o hash.o  testing.o