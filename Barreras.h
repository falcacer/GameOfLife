#ifndef BARRERAS
#define BARRERAS

#include <pthread.h>

/* Definición de la estructura y sinónimo de tipo. */
struct cond_barrier {
	int count;
	int waiting;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};

typedef struct cond_barrier barrier_t;

/* Inicializar una barrera. */
int barrier_init(barrier_t *barr, unsigned int count);

/* Esperar en una barrera. */
int barrier_wait(barrier_t *barr);

/* Destruir una barrera. */
int barrier_destroy(barrier_t *barr);

#endif /* BARRERAS */
