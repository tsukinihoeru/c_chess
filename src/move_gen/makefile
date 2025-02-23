CC = gcc
CFLAGS = -Wall -Wextra
objects = bitboard.o bitboard_util.o make_move.o pgn_parser.o zobrist_hash.o

all : $(objects)

bitboard_util.o : bitboard.h
bitboard.o : bitboard.h bitboard_data.h
make_move.o : bitboard.h
pgn_parser.o : bitboard.h
zobrist_hash.o : bitboard.h

clean :
	rm $(objects)