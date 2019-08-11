#ifndef DOOM_FIRE_RENDERER_H
#define DOOM_FIRE_RENDERER_H

#include<stdbool.h>

int check_args(int argc, char **argv);
int init_renderer(int width, int height);
int get_max_ignition_value();
void draw_buffer(const int *const buffer, int width, int height);
void wait();
bool exit_requested();
void cleanup_renderer();

#endif
