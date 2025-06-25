CC = g++
CFLAGS = -Wall -Wextra
objects = graphics.o buttons.o board_window.o colors.o move_history.o start_menu.o timer.o

all : $(objects)

graphics.o : graphics.h
buttons.o : graphics.h ../engine/engine.h
board_window.o : ../move_gen/bitboard.h graphics.h ../engine/engine.h
colors.o : graphics.h
move_history.o : graphics.h
start_menu.o : graphics.h
timer.o : graphics.h

clean :
	rm $(objects)