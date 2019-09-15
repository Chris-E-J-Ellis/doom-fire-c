#include <SDL2/SDL.h>
#include <unistd.h>
#include "fire-renderer.h"
#include "fire-engine.h"
#include "fire-palette.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Surface* windowSurface = NULL;
static SDL_Surface* bufferSurface = NULL;
static SDL_Rect renderRect = { .x = 0, .y = 0 };

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

    SDL_CreateWindowAndRenderer(buffer->width, buffer->height, SDL_WINDOW_RESIZABLE, &window, &renderer);
    if(window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    windowSurface = SDL_GetWindowSurface(window);
    if(windowSurface == NULL)
    {
        printf("Window surface could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    bufferSurface = SDL_CreateRGBSurface(0, buffer->width, buffer->height, 32, 0, 0, 0, 0);
    if(bufferSurface == NULL)
    {
        printf("Buffer surface could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GetWindowSize(window, &renderRect.w, &renderRect.h);

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

            Uint8 *bufferPixels = (Uint8 *)bufferSurface->pixels;
            Uint32 *targetPixel = (Uint32 *)(bufferPixels + y * bufferSurface->pitch
                    + x * sizeof(*targetPixel));

            *targetPixel = r << 16 | g << 8 | b;
        }
    }

    SDL_BlitScaled(bufferSurface, NULL, windowSurface, &renderRect);
    SDL_UpdateWindowSurface(window);
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

        // Seems like this can hang out here.
        if (e.type == SDL_WINDOWEVENT)
        {
            if(e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                SDL_FreeSurface(windowSurface);
                windowSurface = SDL_GetWindowSurface(window);
                SDL_GetWindowSize(window, &renderRect.w, &renderRect.h);
            }
        }

    }
    return false;
}

void cleanup_renderer()
{
    SDL_FreeSurface(bufferSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    bufferSurface = NULL;
    renderer = NULL;
    windowSurface = NULL;
    window = NULL;

    SDL_Quit();
}

int get_max_ignition_value()
{
    int paletteSize = get_palette_size();
    return paletteSize - 1;
}
