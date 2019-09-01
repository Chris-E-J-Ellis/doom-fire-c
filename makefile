CC=gcc
OBJDIR=obj
SRCDIR=src
INCDIR=include
CFLAGS=-I$(INCDIR) -Wall -Wextra

_DEPENDENCIES = fire-engine.h fire-renderer.h fire-palette.h
DEPENDENCIES = $(patsubst %,$(INCDIR)/%,$(_DEPENDENCIES))

_OBJ = doom-fire.o fire-engine.o fire-palette.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPENDENCIES)
	$(CC) -c -o $@ $< $(CFLAGS)

all: doom-fire-sdl doom-fire-curses

doom-fire-sdl : $(OBJ) $(OBJDIR)/fire-renderer-sdl.o
	$(CC) -o $@ $^ -lSDL2 $(CFLAGS)

doom-fire-curses : $(OBJ) $(OBJDIR)/fire-renderer-curses.o
	$(CC) -o $@ $^ -lncurses $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o
	rm doom-fire-curses 
	rm doom-fire-sdl
