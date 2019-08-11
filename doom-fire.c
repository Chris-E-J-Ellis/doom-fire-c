#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "fire-engine.h" 
#include "fire-renderer.h"

static volatile bool keepRunning = true;
static void sig_handler(int input) 
{
    keepRunning = false;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Insufficient arguments supplied.");
        printf("Usage: doom-fire [WIDTH] [HEIGHT]");
        return 1;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    if (check_args(argc, argv) != 0)
    {
        return 1;
    }

    int *const screenBuffer = (int *const)malloc(width * height * sizeof(int));

    int initSuccessful = init_renderer(width, height);
    if (initSuccessful != 0)
        return initSuccessful;

    int ignitionValue = get_max_ignition_value(); 
    init_buffer(screenBuffer, width, height, ignitionValue);

    signal(SIGINT, sig_handler);

    while (keepRunning)
    {
        if (exit_requested()) 
            keepRunning = false;

        draw_buffer(screenBuffer, width, height);
        step_fire(screenBuffer, width, height);
        wait();
    }

    free(screenBuffer);
    cleanup_renderer();

    printf("Done...\n");

    return 0;
}
