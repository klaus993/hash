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

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->tabla = malloc(TAM_INICIAL * sizeof(nodo_hash_t));
	if (!hash->tabla) {
		free(hash);
		return NULL;
	}
	for (int i = 0; i < TAM_INICIAL; i++) {
		hash->tabla[i].estado = VACIO;
		hash->tabla[i].clave = NULL;
	}
	hash->destruir_dato = destruir_dato;
	hash->cantidad = 0;
	hash->capacidad = TAM_INICIAL;
	return hash;
}

/*Reibe un puntero a un size_t e incrementa su valor en uno. */
void incrementar(size_t *indice) {
	*indice += 1;
}

/* Recibe una tabla de hash, una clave junto con su valor, y su capacidad e introduce la clave y su valor en la tabla. 
Devuelve true si se debe incrementar el miembro cantidad del hash, false en caso contrario. */
bool guardar(nodo_hash_t *tabla, char *clave, void *dato, size_t capacidad) {
	size_t indice = fhash(clave, (unsigned int)capacidad);
	bool flag = false;
	while (tabla[indice].estado != VACIO) {
		if (tabla[indice].estado == OCUPADO && strcmp(tabla[indice].clave, clave) == 0) {
			tabla[indice].valor = dato;
			return flag;
		}
		if (indice == capacidad - 1) indice = -1;
		incrementar(&indice);
	}
	char clave_copiada[strlen(clave) + 1];
	strcpy(clave_copiada, clave);
	tabla[indice].clave = clave_copiada;
	tabla[indice].estado = OCUPADO;
	tabla[indice].valor = dato;
	flag = true;
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
			guardar(tabla_nueva, hash->tabla[i].clave, hash->tabla[i].valor, hash->capacidad);
		}
		else tabla_nueva[i].estado = VACIO;
	}
	free(hash->tabla);
	hash->tabla = tabla_nueva;
	return true;
}

/* Función que recibe una tabla de hash y una clave. 
Recorre la tabla de hash hasta encontrar la clave y una vez encontrada devuelve
el valor a devolver. */
size_t recorrer(const hash_t *hash, const char *clave){
	size_t indice = fhash(clave, (unsigned int)hash->capacidad);
	size_t cont = 0;
	while (hash->tabla[indice].estado != VACIO) {
	char* falopa= hash->tabla[indice].clave;
#include <stdio.h>
printf("falopa %s\n",falopa);
		if (strcmp(falopa, clave) == 0) return indice;
		if (indice == hash->capacidad - 1) indice = -1;
		cont++;
		if (cont == hash->capacidad) break;
		incrementar(&indice);
	}
	return -1;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if ((hash->cantidad / hash->capacidad) >= FACTOR_CARGA_AGRANDAR) {
		if (!hash_redimensionar(hash, hash->capacidad * FACTOR_REDIMENSION)) return false;
	}
	if(guardar(hash->tabla, (char*)clave, dato, hash->capacidad)) hash->cantidad++;
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
	size_t indice = recorrer(hash, clave);
	if (indice != -1) {
		hash->tabla[indice].estado = BORRADO;
		hash->cantidad--;
	}
	else return NULL;	
	if ((hash->cantidad / hash->capacidad) <= FACTOR_CARGA_ACHICAR) {
		hash_redimensionar(hash, hash->capacidad / FACTOR_REDIMENSION);
	}
	return hash->tabla[indice].valor;	
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	size_t indice = recorrer(hash, clave);
	if (indice != -1) return hash->tabla[indice].valor;
	return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	return recorrer(hash, clave) != -1;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
	for (int i = 0; i < hash->capacidad; i++){
		if (hash->destruir_dato) hash->destruir_dato(hash->tabla[i].valor);
		if (hash->tabla[i].clave) free(hash->tabla[i].clave);
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
	hash_iter_t *iter = malloc(sizeof(hash_iter_t));
	if (!iter) return NULL;
	iter->hash = hash;
	iter->pos = 0;
	if (iter->hash->tabla[iter->pos].estado != OCUPADO) {
		hash_iter_avanzar(iter);
	}
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
	if (hash_iter_al_final(iter)) return false;
	do {
		iter->pos++;
	} while (iter->hash->tabla[iter->pos].estado != OCUPADO);
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	return iter->hash->tabla[iter->pos].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	return iter->pos == iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t* iter) {
	free(iter);
}
