doom-fire-sdl: doom-fire.c fire-engine.c
	gcc -Wall doom-fire.c fire-engine.c -o doom-fire -lSDL2
