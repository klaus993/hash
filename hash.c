#include "hash.h"
#include "f_hash.h"
#include <stdlib.h>
#include <string.h>

#define TAM_INICIAL 10
#define FACTOR_CARGA_AGRANDAR 0.70
#define FACTOR_CARGA_ACHICAR 0.15
#define FACTOR_REDIMENSION 2

/*Definición el tipo estado_t. */
typedef enum estado {VACIO, OCUPADO, BORRADO} estado_t;

/*Definición del struct nodo para la tabla de hash. */
typedef struct nodo_hash {
	char *clave;
	void *valor;
	estado_t estado;
} nodo_hash_t;

struct hash {
	nodo_hash_t *tabla;
	size_t cantidad;
	size_t capacidad;
	hash_destruir_dato_t destruir_dato;
};

/* Recibe un puntero hacia un string y un puntero void hacia un valor y crea un 
nodo con ese string como clave y con ese valor en caso de ser posible, y 
devuelve el nodo. En caso contrario devuelve NULL. */
nodo_hash_t *nodo_hash_crear() {
	nodo_hash_t *nodo = malloc(sizeof(nodo_hash_t));
	if (!nodo) return NULL;
	nodo->estado = VACIO;
	return nodo;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->tabla = malloc(TAM_INICIAL * sizeof(nodo_hash_t));
	if (!hash->tabla) {
		free(hash);
		return NULL;
	}
	nodo_hash_t *nodo;
	for (int i = 0; i < TAM_INICIAL; i++) {
		nodo = nodo_hash_crear();
		if (!nodo) {
			free(hash->tabla);
			free(hash);	
			return NULL;
		}
		hash->tabla[i] = *nodo;
	}
	hash->destruir_dato = destruir_dato;
	hash->cantidad = 0;
	hash->capacidad = TAM_INICIAL;
	return hash;
}

void incrementar(size_t *indice) {
	*indice += 1;
}

/* Recibe una tabla de hash, una clave junto con su valor, la cantidad de elementos
en la tabla y su capacidad e introduce la clave y su valor en la tabla. 
Devuelve true si se debe incrementar el miembro cantidad del hash, false en caso contrario. */
void guardar(nodo_hash_t *tabla, const char *clave, void *dato, size_t cantidad, size_t capacidad) {
	size_t indice = hash(clave, capacidad);
	bool flag;
	while (tabla[indice].estado == OCUPADO) {
		incrementar(&indice);
	}
	if (hash_pertenece(hash, clave)) {  // Está mal, no se puede usar hash_pertenece, ya que recibe un hash por parámetro
		while (strcmp(tabla[indice].clave, clave) != 0) incrementar(&indice);
		if (hash->destruir_dato) hash->destruir_dato(hash->tabla[indice].valor);
		flag = false;
	} else {
		while(tabla[indice].estado != VACIO) incrementar(&indice);
		char clave_copiada[strlen(clave) + 1];  // El "+ 1" es para que entre el \0
		strcpy(copiar, clave_copiada);
		tabla[indice].clave = clave_copiada;
		tabla[indice].estado = OCUPADO;
		flag = true;
	}
	tabla[indice]->valor = dato;
	return flag;
}

/* Recibe la estructura hash y la variable redimension de tipo size_t y duplica
o reduce a la mitad la capacidad de la estructura (rehash). Devuelve la nueva
estructura rehasheada, o NULL en caso de no poder haberse hecho la redimensión. */
bool hash_redimensionar(hash_t* hash, size_t redimension) {
	nodo_hash_t* tabla_nueva = malloc(redimension * sizeof(nodo_hash_t));
	if(redimension > 0 && !tabla_nueva) return false;
	hash->capacidad = redimension;
	for (int i = 0; i < redimension; i++) {
		if (hash->tabla[i].clave && hash->tabla[i].estado == OCUPADO) {
			guardar(tabla_nueva, hash->tabla->[i].clave, hash->tabla[i].valor, hash->cantidad, hash->capacidad);
		}
		free(hash->tabla[i]);
	}
	free(hash->tabla);
	hash->tabla = tabla_nueva;
	return true;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if ((hash->cantidad / hash->capacidad) >= FACTOR_CARGA_AGRANDAR) {
		if (!hash_redimensionar(hash, hash->capacidad * FACTOR_REDIMENSION)) return false;
	}
	if(guardar(hash->tabla, clave, dato, ash->cantidad, hash->capacidad)) hash->cantidad++;
	return true;
}

//CODIGO REPETIDO EN BORRAR, OBTENER Y PERTENECE?
void *hash_borrar(hash_t *hash, const char *clave) { // destruyo dato si es dinamico?
	if (!hash_pertenece(hash, clave)) return NULL;
	size_t indice = hash(clave, hash->capacidad);
	while (hash->tabla[indice] != VACIO) {
		if (strcmp(hash->tabla[indice].clave, clave) == 0) hash->tabla[indice].estado = BORRADO;
		incrementar(&indice);
	}
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	if (!hash_pertenece(hash, clave)) return NULL;
	size_t indice = hash(clave, hash->capacidad);
	while (hash->tabla[indice] != VACIO) {
		if (hash->tabla[indice] == clave) return hash->tabla[indice]->valor;
		incrementar(&indice);
	}
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	size_t indice = hash(clave, hash->capacidad);
	while (hash->tabla[indice] != VACIO) {
		if (hash->tabla[indice] == clave) return true;
		incrementar(&indice);
	}
	return false;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
	for (int i = 0; i < hash->capacidad; i++) {
		if(hash->destruir_dato) hash->destruir_dato(hash->tabla[i]->valor);
		free(hash->tabla[i]);
	}
	free(hash->tabla);
	free(hash);
}

/* Iterador del hash */

struct hash_iter {
	const hash_t *hash;
	size_t pos;
};

hash_iter_t *hash_iter_crear(const hash_t *hash) {
	hash_iter_t *hash_iter = malloc(sizeof(hash_iter_t));
	if (!hash_iter) return NULL;
	hash_iter->hash = hash;
	hash_iter->pos = 0;
	return hash_iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
	if (hash_iter_al_final(hash_iter) return false;
	do{
		hash_iter->pos+;
	} while (iter_hash->hash->tabla[hash_iter->pos)->estado != OCUPADO);
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	return hash_iter->hash->tabla[hash_iter->pos]->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	return hash_iter->pos == hash_iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t* iter) {
	free(hash_iter);
}
