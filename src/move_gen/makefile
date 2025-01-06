CC = gcc
CFLAGS = -Wall -Wextra
objects = bitboard.o bitboard_util.o

all : $(objects)

bitboard_util.o : bitboard.h
bitboard.o : bitboard.h bitboard_data.h

clean :
	rm $(objects)