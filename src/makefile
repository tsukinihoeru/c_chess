CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lncurses
objects = main.o
move_gen_objects = move_gen/bitboard_util.o move_gen/bitboard.o move_gen/make_move.o 
graphics_objects = graphics/graphics.o graphics/buttons.o graphics/board_window.o graphics/colors.o graphics/move_history.o
SUBDIRS = move_gen graphics

all : $(objects)
	@for subdir in $(SUBDIRS); do \
        echo "Making all in $$subdir"; \
        cd $$subdir && $(MAKE) all; \
		cd ..; \
    done
	$(CC) -o out $(objects) $(move_gen_objects) $(graphics_objects) $(LDLIBS)

main.o : move_gen/bitboard.h graphics/graphics.h

clean :
	rm $(objects)
	rm out
	@for subdir in $(SUBDIRS); do \
        echo "Cleaning all in $$subdir"; \
        cd $$subdir && $(MAKE) clean; \
		cd ..; \
    done