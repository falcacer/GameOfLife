simulador: Game.c Board.o Barreras.o
	gcc -o simulador Game.c Board.o Barreras.o -pthread

Board.o: Board.c Board.h
	gcc -c Board.c

Barreras.o: Barreras.c Barreras.h
	gcc -c Barreras.c

clean:
	rm -f simulador *.o

