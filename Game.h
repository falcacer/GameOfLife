#include "Board.h"

/******************************************************************************/
/* Representamos las células vivas como 'O' y las muertas como 'X'. */
#define ALIVE 'O'
#define DEAD 'X'
/******************************************************************************/

/* La estructura de un juego es simplemente un tablero y la cantidad de veces
   que se va a iterar. */
struct _game {
  board_t board;
  unsigned int cycles;
};

/* Estructura para pasarle argumento a la función del hilo. */
struct workerInfo_ {
	board_t *bOrig, *bDest;
	unsigned int from, size, goal;
};

typedef struct _game game_t;
typedef game_t* Game;
typedef struct workerInfo_ workerInfo_t;
/******************************************************************************/

/* Cargamos el juego desde un archivo. */
Game loadGame(char *filename);

/* Guardamos el tablero 'board' en el archivo 'filename'. */
void writeBoard(board_t board, char *filename);

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la
   cantidad de ciclos indicados en 'cycles' en 'nuprocs' unidades de
   procesamiento. */
board_t *congwayGoL(board_t *board, unsigned int cycles, const int nuproc);
