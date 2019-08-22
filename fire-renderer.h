#ifndef DOOM_FIRE_RENDERER_H
#define DOOM_FIRE_RENDERER_H

#include <stdbool.h>
#include "fire-engine.h"

int process_additional_args(int argc, char **argv);
int init_renderer(const DoomFireBuffer *const buffer);
int get_max_ignition_value();
void draw_buffer(DoomFireBuffer *const buffer);
void wait();
bool exit_requested();
void cleanup_renderer();

#endif
