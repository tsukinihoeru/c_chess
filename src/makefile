CC = gcc
CFLAGS = -Wall -Wextra
objects = main.o bitboard.o

all : $(objects)
	$(CC) -o out $(objects)

bitboard.o : bitboard.h
main.o : bitboard.h

clean :
	rm out $(objects)