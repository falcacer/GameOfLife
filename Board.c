#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Board.h"

/* Creación del tablero. */
int board_init(board_t *board, size_t row, size_t col)
{
	board->row = row;
	board->col = col;
	board->tab = malloc(sizeof(char) * (col * row));
	return 0;
}

/* Creación del tablero con un elemento por default. */
int board_init_def(board_t *board, size_t row, size_t col, char def)
{
	assert(! board_init(board, col, row) );
	for (int i = 0; i < row * col; i++) board->tab[i] = def;
	return 0;
}

/* Leer el tablero en una posición (col, row). */
char board_get(board_t board, size_t row, size_t col)
{
	return board.tab[board.col * row + col];
}

/* Asignarle un valor 'val' a la posición (col, row) del tablero. */
int board_set(board_t board, size_t row, size_t col, char val)
{
	board.tab[board.col * row + col] = val;
	return 0;
}

/* Leer de una lista de caracteres e interpretarla como un tablero. */
int board_load(board_t *board, char *str)
{
	for (int i = 0; i < board->col * board->row; i++)
		board->tab[i] = str[i];
	return 0;
}

/* Función para mostrar el tablero */
void board_show(board_t board)
{
	for (int i = 0; i < board.row; i++){
		for (int j = 0; j < board.col; j++){
			printf("%c", board_get(board,i,j));
		}
		printf("\n");
	}
}

/* Destroy board */
void board_destroy(board_t *board)
{
	free(board->tab);
}

int board_getInt(board_t board, size_t col, size_t row)
{
	char value = board_get(board,col,row);
	if (value == 'O') return 1;
	else return 0;
}

int getNeighbors(board_t board, size_t row, size_t col)
{
	return
	board_getInt(board,up(row,board.row),col) +
	board_getInt(board,up(row,board.row),right(col,board.col)) +
	board_getInt(board,row,right(col,board.col)) +
	board_getInt(board,down(row,board.row),right(col,board.col)) +
	board_getInt(board,down(row,board.row),col) +
	board_getInt(board,down(row,board.row),left(col,board.col)) +
	board_getInt(board,row,left(col,board.col)) +
	board_getInt(board,up(row,board.row),left(col,board.col));
}

void updateSingle(board_t *boardOrigin, board_t *boardDest, size_t row, size_t col)
{
	assert( (col >= 0) && (col < boardOrigin->col) && (row >= 0) && (row < boardOrigin->row) );
	
	int aliveCount = getNeighbors(*boardOrigin, row, col);
	int deadCount = 8 - aliveCount;
	
	/* Actualizamos el valor en la casilla segun las reglas */
	char c = board_get(*boardOrigin, row, col);
	if (c == 'X') {
		if (aliveCount == 3){ 
			board_set(*boardDest, row, col, 'O');
		}
		else {
			board_set(*boardDest, row, col, 'X');
		}
	}
	else /* c=='O' */ {
		if (aliveCount == 2 || aliveCount == 3) {
			board_set(*boardDest, row, col, 'O');
		}
		else {
			board_set(*boardDest, row, col, 'X');
		}
	}
}

void updateBoard(board_t *boardOrigin, board_t *boardDest, int from, int size)
{
	int i, j;
	for (int k = from; k < from + size; k++) {
		i = k / boardOrigin->col;
		j = k % boardOrigin->col;
		updateSingle(boardOrigin, boardDest, i, j);
	}
}
