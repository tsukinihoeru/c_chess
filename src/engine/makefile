CC = g++
CFLAGS = -Wall -Wextra
objects = simple_evaluate.o search.o sort_moves.o

all : $(objects)

simple_evaluate.o : ../move_gen/bitboard.h evaluate.h
search.o : engine.h evaluate.h
sort_moves.o : engine.h

clean :
	rm $(objects)