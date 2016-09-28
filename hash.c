#include "hash.h"
#include "f_hash.h"
#include <stdlib.h>
#include <string.h>

#define TAM_INICIAL 10
#define FACTOR_REDIMESION 0.70

/*Definición el tipo estado_t. */
typedef enum = {VACIO, OCUPADO, BORRADO} estado_t;

/*Definición del struct nodo para la tabla de hash*/
typedef struct nodo_hash {
	char *clave;
	void *valor;
	estado_t estado;
} nodo_hash_t;

struct hash {
	nodo_hash_t **tabla;
	size_t cantidad;
	size_t capacidad;
	hash_destruir_dato_t destruir_dato;
};

/* Recibe un puntero hacia un string y un puntero void hacia un valor y crea un 
nodo con ese string como clave y con ese valor en caso de ser posible, y 
devuelve el nodo. En caso contrario devuelve NULL. */
nodo_hash_t* nodo_hash_crear(void) {
	nodo_hash_t* nodo = malloc(sizeof(nodo_hash_t));
	if (!nodo) return NULL;
	nodo->estado = VACIO;
	return nodo;
}

/* Recibe la estructura hash y la variable redimension de tipo size_t y duplica
o reduce a la mitad la capacidad de la estructura (rehash). Devuelve la nueva
estructura rehasheada, o NULL en caso de no poder haberse hecho la redimensión. */
bool hash_redimensionar(hash_t* hash, size_t redimension) {
	nodo_hash_t* tabla_nueva = malloc(redimension * sizeof(nodo_hash_t);
	if(redimension > 0 && !tabla_nueva) return false;
	hash->capacidad = redimension;
	for (int i = 0; i < redimension; i++) {
		if (hash->tabla[i]->clave && hash->tabla->estado == OCUPADO) {
			guardar(tabla_nueva, hash->tabla->[i]->clave, hash->tabla[i]->valor, 
			hash->cantidad, hash->capacidad);
		}
		free(hash->tabla[i]);
	free(hash->tabla);
	hash->tabla = tabla_nueva;
	}
	return true;
}
/* Recibe una tabla de hash, una clave junto con su valor, la cantidad de elementos
en la tabla y su capacidad e introduce la clave y su valor en la tabla. */
void guardar(nodo_hash_t **tabla, const char *clave, void *dato, size_t cantidad, size_t capacidad) {
	size_t indice = hash(clave, capacidad);
	while (tabla[indice]->estado == OCUPADO) {
		indice++;
	}
	if (hash_pertenece(hash, clave)) {
		while (tabla[indice]->clave != clave) indice++;
	}
	else {
		while(tabla[indice]->estado != VACIO) indice++;
		tabla[indice]->clave = clave;
		tabla[indice]->estado = OCUPADO;
		cantidad++;
	}
	tabla[indice]->valor = dato;
}
//Hacer el strcpy y la funcion q aumenta pasos que no me acuerdo somo se llamaba
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->tabla = malloc(TAM_INICIAL * sizeof(nodo_hash_t*));
	if (!hash->tabla) {
		free(hash);
		return NULL;
	}
	for (int i = 0; i < TAM_INICIAL; i++) {
		nodo_hash_t* nodo = nodo_crear();
		if (!nodo) {
			free(hash->tabla);
			free(hash);	
			return NULL;
		}
		hash->tabla[i] = nodo;
	}
	if (!destruir_dato) hash->destruir_dato = NULL;
	else hash->destruir_dato = destruir_dato;
	hash->cantidad = 0;
	hash->capacidad = TAM_INICIAL;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if ((hash->cantidad / hash->capacidad) >= FACTOR_REDIMENSION) {
		if (!hash_redimensionar(hash, hash->capacidad * 2)) return false;
	}
	guardar(hash->tabla, clave, dato, hash->cantidad, hash->capacidad);
	return true;
}
//CODIGO REPETIDO EN BORRAR, OBTENER Y PERTENECE?
void *hash_borrar(hash_t *hash, const char *clave) {// destruyo dato si es dinamico?
	if (!hash_pertenece(hash, clave)) return NULL;
	size_t indice = hash(clave, hash->capacidad);
	while (hash->tabla[indice] != VACIO) {
		if (hash->tabla[indice] == clave) {
			void *aux = hash->tabla[indice]->valor;
			if(hash->destruir_dato) hash->destruir_dato(aux);
		indice++;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	if (!hash_pertenece(hash, clave)) return NULL;
	size_t indice = hash(clave, hash->capacidad);
	while (hash->tabla[indice] != VACIO) {
		if (hash->tabla[indice] == clave) return hash->tabla[indice]->valor;
		indice++;
	}
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	size_t indice = hash(clave, hash->capacidad);
	while (hash->tabla[indice] != VACIO) {
		if (hash->tabla[indice] == clave) return true;
		indice++;
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
	if (hash_iter_al_final) return false;
	hash_iter->pos++;
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	return hash_iter->hash->tabla[hash_iter->pos]->clave;// y si clave no está?
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	return hash_iter->pos == hash_iter->hash->capacidad;//el final es cuando estoy en la ultima posicion del vector?
}

void hash_iter_destruir(hash_iter_t* iter) {
	free(hash_iter);
}
