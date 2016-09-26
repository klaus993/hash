#include "hash.h"
#include <stdlib.h>

/*Definicion del struct nodo para la tabla de hash*/
typedef struct nodo_hash{
	void* dato;
	struct nodo_hash* sig;
} nodo_hash_t;

struct hash{
	

