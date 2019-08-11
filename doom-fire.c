#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "fire-engine.h" 

void draw_buffer(int *buffer, int width, int height, SDL_Renderer *renderer);

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Insufficient arguments supplied.");
        printf("Usage: doom-fire [WIDTH] [HEIGHT]\n");
        return 1;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    int *screenBuffer = (int*)malloc(width * height * sizeof(int));
    int paletteSize = sizeof(DoomRGBValues)/sizeof(Uint8) / 3;
    init_buffer(screenBuffer, width, height, paletteSize - 1);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if(SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    if( window == NULL )
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
                quit = true;
        }

        if (quit) 
            break;

        draw_buffer(screenBuffer, width, height, renderer);
        step_fire(screenBuffer, width, height);

        usleep(10000);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void draw_buffer(int *buffer, int width, int height, SDL_Renderer *renderer)
{
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            int pixel = buffer[x + (y * width)];
            int paletteIndex = pixel * 3;
            Uint8 r = DoomRGBValues[paletteIndex];
            Uint8 g = DoomRGBValues[paletteIndex + 1];
            Uint8 b = DoomRGBValues[paletteIndex + 2];

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_RenderPresent(renderer);
}
