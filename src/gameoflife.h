#pragma once

#include <stdbool.h>


typedef struct {
    unsigned int width, height;
    float init_alive_percentage;
    bool *current_grid, *grid;
    bool is_paused, should_reset, is_menu_active;
} GameOfLife;


int init_gameoflife(GameOfLife* g, unsigned int width, unsigned int height, float alive_percentage);
void deinit_gameoflife(GameOfLife* g);
void step_gameoflife(GameOfLife* g);
void render_gameoflife(GameOfLife* g);

