#include <time.h>
#include <stdlib.h>
#include "fire-engine.h" 

static void spread_fire(int *const buffer, int sourcePosition, int width);

void create_buffer(int width, int height, DoomFireBuffer **buffer)
{
    *buffer = malloc(sizeof(DoomFireBuffer)); 
    (*buffer)->width = width;
    (*buffer)->height = height;
    (*buffer)->buffer = malloc(width * height * sizeof(int));
}

void destroy_buffer(DoomFireBuffer **const buffer)
{
    free((*buffer)->buffer);
    free(*buffer);
}

void init_buffer(DoomFireBuffer *const buffer, const int ignitionValue)
{
    srand(time(NULL));

    for (int i = 0; i < buffer->height * buffer->width; i++) 
    {
        buffer->buffer[i] = 0;
    }

    for (int i = (buffer->height - 1) * buffer->width; i < buffer->height * buffer->width; i++) 
    {
        buffer->buffer[i] = ignitionValue;
    }
}

void step_fire(DoomFireBuffer *const buffer)
{
    for (int x = 0; x < buffer->width; x++) 
    {
        for (int y = 1; y < buffer->height; y++) 
        {
            int bufferPos = (y * buffer->width) + x;
            spread_fire(buffer->buffer, bufferPos, buffer->width);
        }
    }
}

static void spread_fire(int *const buffer, int sourcePosition, int width)
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
