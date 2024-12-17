CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lncurses
objects = main.o bitboard.o graphics.o

all : $(objects)
	$(CC) -o out $(objects) $(LDLIBS)

bitboard.o : bitboard.h
main.o : bitboard.h graphics.h
graphics.o : graphics.h

clean :
	rm out $(objects)