#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "Board.h"
#include "Barreras.h"

/* Variable global para el control de hilos */
barrier_t barreraGlobal;

/* Cargamos el juego desde un archivo */

Game loadGame(char *filename){
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

  while(fscanf(input,"%s",buffer) != EOF)
  {
    for(int i = 0; i < strlen(buffer);i++)
    {
      if(buffer[i] == ALIVE){
        board_set(board,fil,i,ALIVE);
      }
      else{
        board_set(board,fil,i,DEAD);
      }
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


/* Guardamos el tablero 'board' en el archivo 'filename' */

void writeBoard(board_t board,char *filename){
  FILE *output;
  output = fopen(filename,"w");
  for(size_t i = 0; i < board.row; i++){
    for(size_t j = 0; j < board.col; j++){
      fprintf(output,"%c",board_get(board,i,j));
    }
    fprintf(output,"%c",'\n');
  }

  fclose(output);
}

/*
 * Un trabajador espera dos porciones de tablero a trabajar: la parte
 * del tablero origen y la parte del tablero destino.
 */

// void * work(void * arg)
// {
// 	workerInfo_t * info = (workerInfo_t*) arg;
	
// 	/* Acá asumimos que:
// 		* info->bOrig es el board inicial que tiene 'X's y 'O's en tablero.
// 		* info->bDest no tiene nada en el tablero inicialmente.
// 		* info->from es el primer indice del tablero en donde tiene que trabajar.
// 		* info->size es el tamanio del arreglo que le corresponde.
// 		* info->goal son los ciclos que tiene que hacer el juego.
// 	*/
	
// 	printf("%d trabajando!\n", info->from / 100);
	
// 	/* Variable para el swap */
// 	Board temp = NULL;
	
// 	for (int i = 0; i < info->goal; i++) {
// 		/* Hacer el trabajo que le corresponde */
// 		updateBoard(info->bOrig, info->bDest, info->from, info->size);

// 		/* Esperamos en la barrera */
// 		assert(! barrier_wait(&barreraGlobal) );
		
// 		/* Hacemos el swap */
// 		temp = info->bOrig;
// 		info->bOrig = info->bDest;
// 		info->bDest = temp;
// 	}
	
// 	return NULL;
	
// }

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la cantidad de
ciclos indicados en 'cycles' en 'nuprocs' unidades de procesamiento*/
// board_t *congwayGoL(board_t *board, unsigned int cycles, const int nuproc)
// {
// 	assert( nuproc > 0 );
	
// 	/* Inicializar b1 y b2 */
// 	board_t *b1 = board;
// 	board_t *b2 = malloc(sizeof(board_t));
	
// 	/* Inicializamos la barrera */
// 	assert(! barrier_init(&barreraGlobal, nuproc) );
	
// 	/* Inicializamos los hilos */
// 	pthread_t workers[nuproc];
// 	workerInfo_t ** configInicial = malloc(sizeof(workerInfo_t*) * nuproc);
// 	int boardSize = ((int) board->row) * ((int) board->col);
// 	int block = boardSize / nuproc;
// 	int littleblock = boardSize % nuproc;
// 	for (int i = 0; i < nuproc; i++){
		
// 		configInicial[i] = malloc(sizeof(workerInfo_t));
// 		configInicial[i]->bOrig = b1;
// 		configInicial[i]->bDest = b2;
// 		configInicial[i]->goal = cycles;
// 		configInicial[i]->from = i * block;
// 		configInicial[i]->size = i == nuproc - 1 ? block + littleblock : block;
		
// 		assert(! pthread_create(&workers[i], NULL, work, (void *) configInicial[i]) );
		
// 	}
	
// 	/* Esperamos a que los hilos terminen */
// 	for (int i = 0; i < nuproc; i++)
// 		assert(! pthread_join(workers[i], NULL) );
	
// 	if (cycles % 2 == 0) return b1;
// 	else return b2;
// }

int main()
{
	// Iniciamos un tablero
	// board_t b;
	// board_t *resultado;
	// char res[400];
	
	// assert(! board_init_def(&b, 20, 20, 'X'));
	
	// assert(! board_set(b, 10, 9, 'O') );
	// assert(! board_set(b, 10, 10, 'O') );
	// assert(! board_set(b, 10, 11, 'O') );
	
	// board_show(b, res);
	// resultado = congwayGoL(&b, 100, 4);
	// board_show(*resultado, res);
	char res[100];
  Game game = loadGame("Ejemplo.game");
  printf("%d\n",game->cycles);
  board_show(game->board,res);
  printf("%d\n",getNeighbors(game->board,0,2));
  writeBoard(game->board,"salida.txt");
  free(game);
	
	return 0;
}
