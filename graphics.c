#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_HEIGHT 100
#define SCREEN_WIDTH 100
#define CUSTOM_PALETTE_SIZE 37

static int *screenBuffer;
static Uint8 doomRGBValues[] = {
    0x07,0x07,0x07,
    0x1F,0x07,0x07,
    0x2F,0x0F,0x07,
    0x47,0x0F,0x07,
    0x57,0x17,0x07,
    0x67,0x1F,0x07,
    0x77,0x1F,0x07,
    0x8F,0x27,0x07,
    0x9F,0x2F,0x07,
    0xAF,0x3F,0x07,
    0xBF,0x47,0x07,
    0xC7,0x47,0x07,
    0xDF,0x4F,0x07,
    0xDF,0x57,0x07,
    0xDF,0x57,0x07,
    0xD7,0x5F,0x07,
    0xD7,0x5F,0x07,
    0xD7,0x67,0x0F,
    0xCF,0x6F,0x0F,
    0xCF,0x77,0x0F,
    0xCF,0x7F,0x0F,
    0xCF,0x87,0x17,
    0xC7,0x87,0x17,
    0xC7,0x8F,0x17,
    0xC7,0x97,0x1F,
    0xBF,0x9F,0x1F,
    0xBF,0x9F,0x1F,
    0xBF,0xA7,0x27,
    0xBF,0xA7,0x27,
    0xBF,0xAF,0x2F,
    0xB7,0xAF,0x2F,
    0xB7,0xB7,0x2F,
    0xB7,0xB7,0x37,
    0xCF,0xCF,0x6F,
    0xDF,0xDF,0x9F,
    0xEF,0xEF,0xC7,
    0xFF,0xFF,0xFF};

void DrawBuffer(int width, int height, SDL_Renderer *renderer);
void InitColours();
void InitCustomColours();
void InitBuffer(int width, int height, int ignitionValue);
void StepFire(int width, int height);
void SpreadFire(int sourcePosition, int width);

int main()
{
    int width = 100;
    int height = 100;
    screenBuffer = (int*)malloc(width * height * sizeof(int));
    InitBuffer(width, height, CUSTOM_PALETTE_SIZE - 1);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_CreateWindowAndRenderer(SCREEN_HEIGHT, SCREEN_WIDTH, 0, &window, &renderer);
    if( window == NULL )
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Wait before drawing on the window, seemingly KDE related 
    SDL_Delay(100);

    for (Uint8 i = 0;i < 100; i++)
    {
        DrawBuffer(width, height, renderer);
        StepFire(width, height);

        usleep(10000);

        //SDL_SetRenderDrawColor(renderer, i, i, i, 255);
        //SDL_RenderDrawPoint(renderer, i, i);
        //SDL_RenderDrawPoint(renderer, 255-i, i);
        //SDL_RenderPresent(renderer);
        //SDL_RenderClear(renderer);
        //SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void DrawBuffer(int width, int height, SDL_Renderer *renderer)
{
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            int pixel = screenBuffer[x + (y * width)];
            int paletteIndex = pixel * 3;
            Uint8 r = doomRGBValues[paletteIndex];
            Uint8 g = doomRGBValues[paletteIndex + 1];
            Uint8 b = doomRGBValues[paletteIndex + 2];

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_Delay(10);
    SDL_RenderPresent(renderer);
}

void InitBuffer(int width, int height, int ignitionValue)
{
    for (int i = 0; i < height * width; i++) 
    {
        screenBuffer[i] = 0;
    }

    for (int i = (height - 1) * width; i < height * width; i++) 
    {
        screenBuffer[i] = ignitionValue;
    }
}

void StepFire(int width, int height)
{
    for (int x = 0; x < width; x++) 
    {
        for (int y = 1; y < height; y++) 
        {
            int bufferPos = (y * width) + x;
            SpreadFire(bufferPos, width);
        }
    }
}

void SpreadFire(int sourcePosition, int width)
{
    int pixel = screenBuffer[sourcePosition];

    if (pixel <= 0)
    {
        screenBuffer[sourcePosition - width] = 0; 
    }
    else
    {
        int decay = rand() & 3;
        int destinationPosition = (sourcePosition - width) - decay + 1;
        if (destinationPosition < 0)
            return;

        screenBuffer[destinationPosition] = pixel - (decay & 1);
    }
}
