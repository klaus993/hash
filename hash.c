#include "hash.h"
#include <stdlib.h>

#define TAM_INICIAL 10

/*Definición el tipo estado_t. */
typedef enum = {VACIO, OCUPADO, BORRADO} estado_t;

/*Definición del struct nodo para la tabla de hash*/
typedef struct nodo_hash {
	char *clave;
	void *valor;
	estado_t estado;
} nodo_hash_t;

struct hash {
	nodo_hash_t **vector;
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
	hash_t* hash_nuevo = hash_crear(hash->destruir_dato);/*Problema, TAMINICIAL en crear es global*/
	if(redimension > 0 && hash_nuevo) return false;
	hash->capacidad = redimension;
	for ( int i = 0; i < redimension; i++) {/*hay que rehashear espacios con estado BORRADO?*/
		if (hash->vector[i]->clave)
		hash_guardar(hash_nuevo, hash->vector[i]->clave, hash->vector[i]->valor);
	}
	return true;
}
	
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->vector = malloc(TAM_INICIAL * sizeof(nodo_hash_t*));
	if (!hash->vector) {
		free(hash);
		return NULL;
	}
	for (int i = 0; i < TAM_INICIAL; i++) {
		nodo_hash_t* nodo = nodo_crear();
		if (!nodo) {
			free(hash->vector);
			free(hash);	
			return NULL;
		}
		hash->vector[i] = nodo;
	}
	if (!destruir_dato) hash->destruir_dato = NULL;
	else hash->destruir_dato = destruir_dato;
	hash->cantidad = 0;
	hash->capacidad = TAM_INICIAL;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if ((hash->cantidad / hash->capacidad) >= 0.7) {
		if (!hash_redimensionar(hash, hash->tamaño * 2)) return false;
	}
	size_t indice = hash(clave, hash->capacidad);//FUNCION A CONSEGUIR
	if (hash_obtener(hash, clave)){
		while (hash->vector[indice]->clave != clave) indice++;
	else {
		while(hash->vector[indice]->estado != VACIO) indice++;
		hash->vector[indice]->clave = clave;
		hash->vector[indice]->estado = OCUPADO;
	}
	hash->vector[indice]->valor = valor;
	hash->cantidad++; 
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
	return hash->cantidad;
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
