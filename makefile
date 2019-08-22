doom-fire-sdl: doom-fire.c fire-engine.c fire-renderer-sdl.c
	gcc -Wall doom-fire.c fire-engine.c fire-renderer-sdl.c -o doom-fire-sdl -lSDL2

doom-fire-curses: doom-fire.c fire-engine.c fire-renderer-curses.c
	gcc -Wall doom-fire.c fire-engine.c fire-renderer-curses.c -o doom-fire-curses -lncurses
