#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fire-engine.h"
#include "fire-renderer.h"

#define MAX_PALETTE_SIZE 37

static int useCustomColours;
static int pixelPalette[MAX_PALETTE_SIZE];
static int paletteSize = 0;

static void init_colours();
static void init_custom_colours();

int process_additional_args(int argc, char **argv)
{
    if (argc < 1)
    {
        printf("    -c    Use a custom colour palette");
        printf("          N.B. Using custom palette will alter terminal colours.\n");
        return 0;
    }

    if (argv[0][1] == 'c')
        useCustomColours = 1; 

    return 0;
}

int init_renderer(const DoomFireBuffer *const buffer)
{
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);

    if (useCustomColours == 1)
    {
        init_custom_colours();
    }
    else
    {
        init_colours();
    }

    clear();
    return 0;
}

void draw_buffer(DoomFireBuffer *const buffer)
{
    for (int y = 0; y < buffer->height; y++) 
    {
        for (int x = 0; x < buffer->width; x++) 
        {
            int pixel = buffer->buffer[x + (y * buffer->width)];
            int colorPair = pixelPalette[pixel];
            char output = ' '; 
            attron(COLOR_PAIR(colorPair));
            mvaddch(y, x, output);
            attroff(COLOR_PAIR(colorPair));
        }
    }
    refresh();
}

void wait()
{
    usleep(10000);
}

bool exit_requested()
{
    return false;
}

void cleanup_renderer()
{
    clear();
    endwin();
}

void init_colours()
{
    printw("Test Palette:\n");
    for (int i = 0; i < 256; i++)
    {
        init_pair(i, COLOR_BLACK, i);
        attron(COLOR_PAIR(i));
        printw("%i ", i);
        attroff(COLOR_PAIR(i));
    }

    int reducedPalette[] = {16, 233, 234, 52, 52, 88, 124, 160, 196, 202, 208, 215, 220, 227, 229, 230, 15};
    paletteSize = sizeof(reducedPalette)/sizeof(int);

    printw("\nSelected Palette:\n");
    for (int i = 0; i < paletteSize; i++)
    {
        pixelPalette[i] = reducedPalette[i];
        attron(COLOR_PAIR(pixelPalette[i]));
        printw("%i ", i);
        attroff(COLOR_PAIR(pixelPalette[i]));
    }

    refresh();
    usleep(2000000);
}

void init_custom_colours()
{
    printw("Selected Palette:\n");
    float colorScalefactor = 1000.0/256;
    int availableRGBValues = sizeof(DOOM_RGB_VALUES)/sizeof(int)/3;
    paletteSize = availableRGBValues;

    for (int i = 0; i < availableRGBValues; i++)
    {
        int colorIndex = i + 50; // Not saving the terminal colours, try not to hit the common ones.
        int paletteIndex = i * 3;
        float red = (float)DOOM_RGB_VALUES[paletteIndex] * colorScalefactor;
        float green = (float)DOOM_RGB_VALUES[paletteIndex + 1] * colorScalefactor; 
        float blue = (float)DOOM_RGB_VALUES[paletteIndex + 2] * colorScalefactor;

        init_color(colorIndex, (int)red, (int)green, (int)blue);
        init_pair(colorIndex, COLOR_BLACK, colorIndex);
        pixelPalette[i] = colorIndex;

        attron(COLOR_PAIR(colorIndex));
        printw("%i ", i);
        attroff(COLOR_PAIR(colorIndex));
    }

    refresh();
    usleep(2000000);
}

int get_max_ignition_value()
{
    return paletteSize - 1; 
}
