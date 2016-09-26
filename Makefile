CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
LFLAGS =  
FILEOUT = pruebas.out
VALGRIND = valgrind
TDA = hash
PRUBEAS = pruebas_catedra
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes
VALGRIND_V = $(VALGRIND) $(VFLAGS)

all: main.o $(PRUEBAS).o $(TDA).o $(EXTRA_O) testing.o
	$(CC) $(CFLAGS) -o $(FILEOUT) main.o $(PRUEBAS).o $(TDA).o $(EXTRA_O) testing.o $(LFLAGS) 

main.o: main.c $(TDA).h testing.h
	$(CC) $(CFLAGS) -c -o main.o main.c $(LFLAGS)

$(PRUEBAS).o: $(PRUEBAS).c $(TDA).h $(EXTRA_H) testing.h
	$(CC) $(CFLAGS) -c -o $(PRUEBAS).o $(PRUEBAS).c $(LFLAGS)

$(TDA).o: $(TDA).c $(TDA).h 
	$(CC) $(CFLAGS) -c -o $(TDA).o $(TDA).c $(LFLAGS)

#pila.o: pila.c pila.h
#	$(CC) $(CFLAGS) -c -o pila.o pila.c $(LFLAGS)

testing.o: testing.c testing.h
	$(CC) $(CFLAGS) -c -o testing.o testing.c $(LFLAGS)

clean:
	rm *.o
	rm -r *.dSYM
	rm *.out

test:
	$(VALGRIND_V) ./$(FILEOUT) 

run:
	./$(FILEOUT)
