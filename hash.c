#include "hash.h"
#include "f_hash.h"
#include <stdlib.h>
#include <string.h>

#define TAM_INICIAL 100
#define FACTOR_CARGA_AGRANDAR 0.70
#define FACTOR_CARGA_ACHICAR 0.15
#define FACTOR_REDIMENSION 2

/*
const int TAM_INICIAL = 100;
const double FACTOR_CARGA_AGRANDAR = 0.7;
const double FACTOR_CARGA_ACHICAR = 0.15;
const int FACTOR_REDIMENSION = 2; */

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


/*
*/
nodo_hash_t *crear_tabla(size_t tamanio) {
	nodo_hash_t *tabla = malloc(tamanio * sizeof(nodo_hash_t));
	if (!tabla) {
		return NULL;
	}
	for (int i = 0; i < tamanio; i++) {
		tabla[i].estado = VACIO;
	}
	return tabla;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->tabla = crear_tabla(TAM_INICIAL);
	if (!hash->tabla) {
		free(hash);
		return NULL;
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

/* Recibe la estructura hash y la variable redimension de tipo size_t y re hashea
todos los datos en una nueva tabla del tamaño indicado por parámetro.
Pre: El hash fue creado.
Post: Devuelve true si se rehasheó correctamente, o false en caso de no poder haberse creado la tabla nueva.
	  La tabla vieja se liberó de la memoria, y la tabla nueva es la nueva tabla del hash.
*/
bool hash_redimensionar(hash_t* hash, size_t redimension) {
	nodo_hash_t *tabla_nueva = crear_tabla(redimension);
	size_t indice_nuevo;
	if (!tabla_nueva) return false;
	if (hash->cantidad != 0) {
		for (int i = 0; i < hash->capacidad; i++) {
			if (hash->tabla[i].estado == OCUPADO) {
				indice_nuevo = fhash(hash->tabla[i].clave, (unsigned int)redimension);
				while (tabla_nueva[indice_nuevo].estado != VACIO) {
					if (indice_nuevo == hash->capacidad - 1) indice_nuevo = -1;
					incrementar(&indice_nuevo);
				}
				tabla_nueva[indice_nuevo] = hash->tabla[i];
			}
		}
	}
	free(hash->tabla);
	hash->capacidad = redimension;
	hash->tabla = tabla_nueva;
	return true;
}

/* Función que recibe una tabla de hash y una clave. 
Recorre la tabla de hash hasta encontrar la clave y una vez encontrada devuelve
el índice en el cual se encuentra la clave, o -1 si no se encuentra. */
size_t recorrer(const hash_t *hash, const char *clave){
	size_t indice = fhash(clave, (unsigned int)hash->capacidad);
	size_t cont = 0;
	while (hash->tabla[indice].estado != VACIO) {
		if (hash->tabla[indice].estado == OCUPADO && strcmp(hash->tabla[indice].clave, clave) == 0) return indice;
		if (indice == hash->capacidad - 1) indice = -1;
		cont++;
		if (cont == hash->capacidad) break;
		incrementar(&indice);
	}
	return -1;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if (((double)hash->cantidad / (double)hash->capacidad) >= (double)FACTOR_CARGA_AGRANDAR) {
		if (!hash_redimensionar(hash, hash->capacidad * FACTOR_REDIMENSION)) return false;
	}
	size_t indice = fhash(clave, (unsigned int)hash->capacidad);
	while (hash->tabla[indice].estado != VACIO) {
		if (hash->tabla[indice].estado == OCUPADO && strcmp(hash->tabla[indice].clave, clave) == 0) {
			if (hash->destruir_dato) hash->destruir_dato(hash->tabla[indice].valor);
			hash->tabla[indice].valor = dato;
			return true;
		}
		if (indice == hash->capacidad - 1) indice = -1;
		incrementar(&indice);
	}
	char *clave_copiada = malloc(sizeof(char) * strlen(clave) + 1);
	strcpy(clave_copiada, clave);
	hash->tabla[indice].clave = clave_copiada;
	hash->tabla[indice].estado = OCUPADO;
	hash->tabla[indice].valor = dato;
	hash->cantidad++;
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
	size_t indice = recorrer(hash, clave);
	if (indice != -1 && ((double)(hash->cantidad - 1) / (double)hash->capacidad) <= (double)FACTOR_CARGA_ACHICAR) {
		hash_redimensionar(hash, hash->capacidad / FACTOR_REDIMENSION);
		indice = recorrer(hash, clave);
	}
	if (indice != -1) {
		free(hash->tabla[indice].clave);
		hash->tabla[indice].estado = BORRADO;
		hash->cantidad--;
	} else {
		return NULL;
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
		if (hash->destruir_dato && hash->tabla[i].estado == OCUPADO) hash->destruir_dato(hash->tabla[i].valor);
		if (hash->tabla[i].estado == OCUPADO) free(hash->tabla[i].clave);
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
	} while (!hash_iter_al_final(iter) && iter->hash->tabla[iter->pos].estado != OCUPADO);
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	if (hash_iter_al_final(iter)) {
		return NULL;
	}
	return iter->hash->tabla[iter->pos].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	return iter->pos == iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t* iter) {
	free(iter);
}
