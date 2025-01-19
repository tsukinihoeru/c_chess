CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lncurses
objects = graphics.o main.o buttons.o board_window.o colors.o move_history.o
subdir_objects = move_gen/bitboard_util.o move_gen/bitboard.o move_gen/make_move.o
SUBDIRS = move_gen

all : $(objects)
	@for subdir in $(SUBDIRS); do \
        echo "Making all in $$subdir"; \
        cd $$subdir && $(MAKE) all; \
    done
	$(CC) -o out $(objects) $(subdir_objects) $(LDLIBS)

graphics.o : graphics.h
main.o : move_gen/bitboard.h graphics.h
buttons.o : graphics.h
board_window.o : move_gen/bitboard.h graphics.h
colors.o : graphics.h
move_history.o : graphics.h

clean :
	rm $(objects)
	@for subdir in $(SUBDIRS); do \
        echo "Cleaning all in $$subdir"; \
        cd $$subdir && $(MAKE) clean; \
    done