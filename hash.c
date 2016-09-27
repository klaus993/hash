#include "hash.h"
#include <stdlib.h>

#define TAM_INICIAL 10

/*Definicion del struct nodo para la tabla de hash*/
typedef struct nodo_hash {
	char *clave;
	void *valor;
} nodo_hash_t;

struct hash {
	nodo_hash_t **vector;
	size_t cantidad;
	size_t tamaño;
	hash_destruir_dato_t destruir_dato;

};

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->vector = malloc(TAM_INICIAL * sizeof(nodo_hash_t*));
	if (!hash->vector) {
		free(hash);
		return NULL;
	}
	if (!destruir_dato) hash->destruir_dato = NULL;
	else hash->destruir_dato = destruir_dato;
	hash->cantidad = 0;
	hash->tamaño = TAM_INICIAL;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
	return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	return true;
}

size_t hash_cantidad(const hash_t *hash) {
	return 0;
}

void hash_destruir(hash_t *hash) {

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
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	return 0;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	return true;
}

void hash_iter_destruir(hash_iter_t* iter) {

}
