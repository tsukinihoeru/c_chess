CC = g++
CFLAGS = -Wall -Wextra
objects = simple_evaluate.o search.o

all : $(objects)

simple_evaluate.o : ../move_gen/bitboard.h evaluate.h
search.o : engine.h evaluate.h

clean :
	rm $(objects)