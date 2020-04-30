#include <assert.h>
#include <pthread.h>
#include "Barreras.h"

/* Operaciones */

/*
 * Creación de una barrera de condición, tomando como argumento la
 * cantidad de hilos que se van a esperar.
 */
int barrier_init(barrier_t *barr, unsigned int count)
{
	barr->count = count;
	barr->waiting = 0;
	assert(! pthread_mutex_init(&barr->mutex, NULL) );
	assert(! pthread_cond_init(&barr->cond, NULL) );
	return 0;
}

/*
 * Función *bloqueante* para esperar a los demás hilos.
 */
int barrier_wait(barrier_t *barr)
{
	assert(! pthread_mutex_lock(&barr->mutex) );
	if (barr->waiting == barr->count - 1) {
		barr->waiting = 0;
		assert(! pthread_mutex_unlock(&barr->mutex) );
		assert(! pthread_cond_broadcast(&barr->cond) );
	}
	else {
		barr->waiting++;
		assert(! pthread_cond_wait(&barr->cond, &barr->mutex) );
		assert(! pthread_mutex_unlock(&barr->mutex) );
	}
	return 0;
}

/*
 * Eliminación de la barrera.
 */
int barrier_destroy(barrier_t *barr)
{
	assert(! barr->waiting );
	assert(! pthread_mutex_destroy(&barr->mutex) );
	assert(! pthread_cond_destroy(&barr->cond) );
	return 0;
}
