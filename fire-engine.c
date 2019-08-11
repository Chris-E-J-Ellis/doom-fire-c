#include "fire-engine.h" 
#include <stdlib.h>

static void spread_fire(int * buffer, int sourcePosition, int width);

void init_buffer(int* buffer, int width, int height, int ignitionValue)
{
    for (int i = 0; i < height * width; i++) 
    {
        buffer[i] = 0;
    }

    for (int i = (height - 1) * width; i < height * width; i++) 
    {
        buffer[i] = ignitionValue;
    }
}

void step_fire(int *buffer, int width, int height)
{
    for (int x = 0; x < width; x++) 
    {
        for (int y = 1; y < height; y++) 
        {
            int bufferPos = (y * width) + x;
            spread_fire(buffer, bufferPos, width);
        }
    }
}

static void spread_fire(int *buffer, int sourcePosition, int width)
{
    int pixel = buffer[sourcePosition];

    if (pixel <= 0)
    {
        buffer[sourcePosition - width] = 0; 
    }
    else
    {
        int decay = rand() & 3;
        int destinationPosition = (sourcePosition - width) - decay + 1;
        if (destinationPosition < 0)
            return;

        buffer[destinationPosition] = pixel - (decay & 1);
    }
}
