#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "fire-engine.h"
#include "fire-renderer.h"

static volatile bool keepRunning = true;
static void sig_handler(int input)
{
    (void)input;
    keepRunning = false;
}

int main(int argc, char **argv)
{
    bool argumentError = false;

    if (argc < 3)
    {
        printf("Insufficient arguments supplied.\n");
        printf("Usage: doom-fire [WIDTH] [HEIGHT]\n");
        argumentError = true;
    }

    if (process_additional_args(argc - 3, &argv[3]) == 1)
    {
        printf("Unable to process additional arguments.");
        argumentError = true;
    }

    if (argumentError)
        return 1;

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    DoomFireBuffer *buffer = NULL;
    create_buffer(width, height, &buffer);

    int initSuccessful = init_renderer(buffer);
    if (initSuccessful != 0)
        return initSuccessful;

    int ignitionValue = get_max_ignition_value();
    init_buffer(buffer, ignitionValue);

    signal(SIGINT, sig_handler);

    while (keepRunning)
    {
        if (exit_requested())
            keepRunning = false;

        draw_buffer(buffer);
        step_fire(buffer);
        wait();
    }

    destroy_buffer(&buffer);
    cleanup_renderer();

    printf("Done...\n");

    return 0;
}
