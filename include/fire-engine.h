#ifndef DOOM_FIRE_ENGINE_H
#define DOOM_FIRE_ENGINE_H

typedef struct DoomFireBuffer {
    int width;
    int height;
    int *buffer;
} DoomFireBuffer;

void create_buffer(int width, int height, DoomFireBuffer **const buffer);
void destroy_buffer(DoomFireBuffer **const buffer);
void init_buffer(DoomFireBuffer *const buffer, int ignitionValue);
void step_fire(DoomFireBuffer *const buffer);

#endif
