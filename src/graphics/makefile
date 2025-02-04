CC = g++
CFLAGS = -Wall -Wextra
objects = graphics.o buttons.o board_window.o colors.o move_history.o

all : $(objects)

graphics.o : graphics.h
buttons.o : graphics.h
board_window.o : ../move_gen/bitboard.h graphics.h
colors.o : graphics.h
move_history.o : graphics.h

clean :
	rm $(objects)