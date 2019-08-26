#include <SDL2/SDL.h>
#include <unistd.h>
#include "fire-renderer.h"
#include "fire-engine.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

int process_additional_args(int argc, char **argv)
{
    (void)argv;

    if (argc > 3)
        return 1;

    return 0;
}

int init_renderer(const DoomFireBuffer *const buffer)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_CreateWindowAndRenderer(buffer->width, buffer->height, 0, &window, &renderer);
    if( window == NULL )
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

void draw_buffer(DoomFireBuffer *const buffer)
{
    for (int y = 0; y < buffer->height; y++) 
    {
        for (int x = 0; x < buffer->width; x++) 
        {
            int pixel = buffer->buffer[x + (y * buffer->width)];
            int paletteIndex = pixel * 3;
            Uint8 r = DOOM_RGB_VALUES[paletteIndex];
            Uint8 g = DOOM_RGB_VALUES[paletteIndex + 1];
            Uint8 b = DOOM_RGB_VALUES[paletteIndex + 2];

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_RenderPresent(renderer);
}

void wait()
{
    usleep(1000);
}

bool exit_requested()
{
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
            return true;
    }
    return false;
}

void cleanup_renderer()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int get_max_ignition_value()
{
    int paletteSize = sizeof(DOOM_RGB_VALUES)/sizeof(uint8_t) / 3;
    return paletteSize - 1;
}
