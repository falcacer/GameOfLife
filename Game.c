#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Game.h"
#include "Board.h"
#include "Barreras.h"

/* Barrera global para el control de hilos */
barrier_t barreraGlobal;

/* Cargamos el juego desde un archivo. */
Game loadGame(char *filename)
{
	FILE *input;
	input = fopen(filename, "r");
	if(input == NULL){
		printf("OCURRIO UN ERROR, POR FAVOR CARGAR EL ARCHIVO DE NUEVO.\n");
		exit(1);
	}
	board_t board;
	char buffer[100];
	int fil = 0;
	fscanf(input,"%s",buffer);
	int ciclos = atoi(buffer);
	fscanf(input,"%s",buffer);
	size_t fila = atoi(buffer);
	fscanf(input,"%s",buffer);
	size_t columna = atoi(buffer);
	board_init(&board,fila,columna);

	while(fscanf(input,"%s",buffer) != EOF) {
		for(int i = 0; i < strlen(buffer);i++) {
			if (buffer[i] == ALIVE)
				board_set(board,fil,i,ALIVE);
			else
				board_set(board,fil,i,DEAD);
		}
		fil++;
	}

	board.row = fila;
	board.col = columna;

	Game newgame = malloc(sizeof(struct _game));
	newgame->cycles = ciclos;
	newgame->board = board;

	return newgame;
}


/* Guardamos el tablero 'board' en el archivo 'filename'. */
void writeBoard(board_t board,char *filename)
{
	FILE *output;
	output = fopen(filename,"w");
	for(size_t i = 0; i < board.row; i++){
		for(size_t j = 0; j < board.col; j++)
			fprintf(output,"%c",board_get(board,i,j));
		fprintf(output,"%c",'\n');
	}

	fclose(output);
}

/* Función que ejecuta un hilo. Un hilo espera dos porciones de tablero
   a trabajar: la parte del tablero origen y la parte del tablero destino. */
void * work(void * arg)
{
	workerInfo_t * info = (workerInfo_t*) arg;
	
	/* Acá asumimos que:
		
		info->bOrig es el tablero inicial que tiene 'X's y 'O's en tablero.
		info->bDest no tiene nada en el tablero inicialmente.
		info->from es el primer índice del tablero en donde tiene que trabajar.
		info->size es el tamaño del arreglo que le corresponde.
		info->goal es la cantidad de ciclos que tiene que hacer el juego.
	
	*/
	
	/* Variable para el swap */
	board_t *temp = NULL;
	
	for (int i = 0; i < info->goal; i++) {
		
		/* Hacer el trabajo que le corresponde */
		updateBoard(info->bOrig, info->bDest, info->from, info->size);
		
		/* Esperamos en la barrera */
		assert(! barrier_wait(&barreraGlobal) );
		
		/* Hacemos el swap */
		temp = info->bOrig;
		info->bOrig = info->bDest;
		info->bDest = temp;
	}
	
	return NULL;
	
}

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la
   cantidad de ciclos indicados en 'cycles' en 'nuprocs' unidades de
   procesamiento. */
board_t *congwayGoL(board_t *board, unsigned int cycles, const int nuproc)
{
	
	/* Inicializar b1 y b2, suponemos que board ya esta inicializado */
	//board_t *b1 = board;
	board_t *boardAux = malloc(sizeof(board_t));
	assert(! board_init_def(boardAux, board->row, board->col, 'X') );
	
	/* Inicializamos las barreras */
	assert(! barrier_init(&barreraGlobal, nuproc) );
	
	/* Inicializamos los hilos */
	pthread_t workers[nuproc];
	workerInfo_t ** configInicial = malloc(sizeof(workerInfo_t*) * nuproc);
	int boardSize = ((int) board->row) * ((int) board->col);
	int block = boardSize / nuproc;
	int littleblock = boardSize % nuproc;
	for (int i = 0; i < nuproc; i++) {
		
		configInicial[i] = malloc(sizeof(workerInfo_t));
		configInicial[i]->bOrig = board;
		configInicial[i]->bDest = boardAux;
		configInicial[i]->goal = cycles;
		configInicial[i]->from = i * block;
		/* Si es el último hilo, quizá tenga que hacer un poco mas de trabajo. */
		configInicial[i]->size = i == nuproc - 1 ? block + littleblock : block;
		
		assert(! pthread_create(&workers[i], NULL, work, (void *) configInicial[i]) );
		
	}
	
	/* Esperamos que los hilos terminen */
	for (int i = 0; i < nuproc; i++)
		assert(! pthread_join(workers[i], NULL) );
	
	/* Si la cantidad de ciclos es par, la solucion esta en el tablero
	   original, sino en el creado adicionalmente. */
	if (cycles % 2 == 0) return board;
	else return boardAux;
}

int main(int argc, char* argv[])
{
	if (argc != 2){
		printf("La cantidad de argumentos es incorrecta, finalizando\n");
		return -1;
	}
	Game game = loadGame(argv[1]);
	
	/* Iniciamos un tablero y ejecutamos. */
	board_t board;
	board = game->board;
	board_t *resultado;

	board_show(board);
	printf("\n");

	resultado = congwayGoL(&board, 1, 4);
	board_show(*resultado);
	writeBoard(*resultado,strcat(argv[1],".final"));
	board_destroy(&game->board);
	board_destroy(resultado);
	free(game);
	
	return 0;
}
