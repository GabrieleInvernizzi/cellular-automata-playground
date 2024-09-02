#include "gameoflife.h"

#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "utils.h"


static inline bool get_cell(GameOfLife* g, unsigned int i, unsigned int j) {
    return g->current_grid[INDEX_2D(i % g->height, j % g->width, g->height)];
}

static inline void set_cell(GameOfLife* g, unsigned int i, unsigned int j, bool val) {
    g->grid[INDEX_2D(i % g->height, j % g->width, g->height)] = val;
}

static void swap_grids_gameoflife(GameOfLife* g) {
    bool* tmp = g->current_grid;
    g->current_grid = g->grid;
    g->grid = tmp;
}

static unsigned int count_alive_neighs(GameOfLife* g, unsigned int i, unsigned int j) {
    unsigned int res = 0;
    for(int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            if (x != 0 || y != 0) { 
                unsigned int mi = mod(i + y, g->height);
                unsigned int mj = mod(j + x, g->width);
                res += (unsigned int)get_cell(g, mi, mj);
            }
        }
    }

    return res;
}

static int reset_gameoflife(GameOfLife* g) {
    unsigned int width = g->width;
    unsigned int height = g->height;
    float alive_perc = g->init_alive_percentage;

    deinit_gameoflife(g);
    return init_gameoflife(g, width, height, alive_perc);
}

static void check_input(GameOfLife* g) {
    if (IsKeyPressed(KEY_SPACE))
        g->is_paused = !g->is_paused;
    if (IsKeyPressed(KEY_R))
        g->should_reset = true;
    if (IsKeyPressed(KEY_M) || IsKeyPressed(KEY_ESCAPE))
        g->is_menu_active = !g->is_menu_active;
}


int init_gameoflife(GameOfLife* g, unsigned int width, unsigned int height, float alive_percentage) {
    g->current_grid = (bool*)malloc((width)*(height)*sizeof(bool));
    g->grid = (bool*)malloc((width)*(height)*sizeof(bool));
    if (!g->grid || !g->current_grid) {
        deinit_gameoflife(g);
        return -1;
    }

    g->should_reset = false;
    g->is_paused = false;
    g->is_menu_active = false;
    g->width = width;
    g->height = height;
    g->init_alive_percentage = alive_percentage;

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if(get_random_float() < alive_percentage) {
                set_cell(g, i, j, true);
            }
        }
    }

    swap_grids_gameoflife(g);

    return 0;
}

void deinit_gameoflife(GameOfLife* g) {
    if(g->current_grid) free(g->current_grid);
    if(g->grid) free(g->grid);
}

void step_gameoflife(GameOfLife* g) {
    check_input(g);

    if (g->should_reset) {
        reset_gameoflife(g);        // TODO: check for error code.
    }
    if (g->is_paused) return;


    for (unsigned int i = 0; i < g->height; i++) {
        for (unsigned int j = 0; j < g->width; j++) {
            unsigned int n_alive_cells = count_alive_neighs(g, i, j);

            if (get_cell(g, i, j)) {
                if (n_alive_cells < 2 || n_alive_cells > 3)
                    set_cell(g, i, j, false);
                else
                    set_cell(g, i, j, true);
            } else {
                if (n_alive_cells == 3)
                    set_cell(g, i, j, true);
                else
                    set_cell(g, i, j, false);
            }
        }
    }

    swap_grids_gameoflife(g);
}

static void render_menu_gameoflife(GameOfLife* g, int r_width, int r_height) {
    DrawRectangle(0, 0, r_width, r_height, ColorAlpha(BLACK, 0.9));     

    int font_size = r_width/15;

    int text_width = MeasureText("WOP menu", font_size);
    DrawText("WIP menu", (r_width - text_width)/2, (r_height-font_size)/2 , font_size, RAYWHITE);
}

void render_gameoflife(GameOfLife* g) {
    int r_width = GetScreenWidth();
    int r_height = GetScreenHeight();

    int scale_width = r_width/g->width;
    int scale_height = r_height/g->height;
    int cell_size_scalar = min(scale_height, scale_width);

    Vector2 cell_size = Vector2Scale(Vector2One(), cell_size_scalar);
    Vector2 offset = 
        Vector2Scale(
            Vector2Subtract(
                (Vector2) {.x=r_width, .y=r_height},
                Vector2Scale((Vector2) {.x=g->width, .y=g->height}, cell_size_scalar)
            ),
            0.5
        );

    for(size_t i = 0; i < g->height; i++) {
        for(size_t j = 0; j < g->width; j++) {
            bool is_alive = get_cell(g, i, j);
            Vector2 pos = 
                Vector2Add(
                    offset,
                    Vector2Multiply((Vector2){.x=j, .y=i}, cell_size)
                );

            DrawRectangleV(pos, cell_size, is_alive ? WHITE : BLACK);
        }
    }


    if (g->is_menu_active) {
        render_menu_gameoflife(g, r_width, r_height);
    }
}

