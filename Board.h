#ifndef BOARD
#define BOARD

#include <stdlib.h>

/* Macros para la implementar la función getNeighbors */
#define up(row,tabrow) ((row ? row : tabrow) - 1)
#define down(row,tabrow) ((row == tabrow -1 ? -1 : row) + 1)
#define left(col,tabcol) ((col ? col : tabcol) -1)
#define right(col,tabcol) ((col == tabcol -1 ? -1 : col) + 1)


/* Definición de la estructura de datos del tablero */
struct _board {
	size_t row;
	size_t col;
	char *tab;
};
typedef struct _board board_t;
typedef board_t* Board;

/**********************************************************************/
/**********************************************************************/

/* FUNCIONES SOBRE EL TABLERO */

/* Creación del tablero. */
int board_init(board_t *board, size_t col, size_t row);

/* Creación del tablero con un elemento por default. */
int board_init_def(board_t *board, size_t col, size_t row, char def);

/* Leer el tablero en una posición (col, row). */
char board_get(board_t board, size_t col, size_t row);

/* Asignarle un valor 'val' a la posición (col, row) del tablero. */
int board_set(board_t board, size_t col, size_t row, char val);

/* Leer de una lista de caracteres e interpretarla como un tablero. */
int board_load(board_t *board, char *str);

/* Función para mostrar el tablero. */
void board_show(board_t board);

/* Destroy board. */
void board_destroy(board_t *board);

/* Devuelve 1 si hay vida en la casilla y 0 en caso contrario. */
int board_getInt(board_t board, size_t col, size_t row);

/**********************************************************************/
/**********************************************************************/

/* FUNCIONES ADICIONALES */

/* Obtener la cantidad de vecinos vivos de una casilla. */
int getNeighbors(board_t board, size_t col, size_t row);

/* Actualizar el valor de una casilla pos en boardOrig a boardDest. */
void updateSingle(board_t *boardOrigin, board_t *boardDest, size_t col, size_t row);

/* Actualizar board desde orig a dest, desde la posicion from, size cantidad de casillas. */
void updateBoard(board_t *boardOrigin, board_t *boardDest, int from, int size);

#endif /* BOARD */
