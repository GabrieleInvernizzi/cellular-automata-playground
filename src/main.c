#include <stdlib.h>
#include <stdbool.h>

#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH (500)
#define SCREEN_HEIGHT (500)

#define WINDOW_TITLE "Cellular Automata Explorer"

#define INDEX_2D(i, j, height) ((i) + (height)*(j))


int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

float get_random_float() {
    return (float)GetRandomValue(0, 100000)/(float)(100001);
}

unsigned int mod(int a, int b) {
    return (a % b + b) % b;
}

typedef struct {
    unsigned int width, height;
    bool *current_grid, *grid;
    bool is_paused;
} GameOfLife;


void deinit_gameoflife(GameOfLife* g);

static inline bool get_cell(GameOfLife* g, unsigned int i, unsigned int j) {
    return g->current_grid[INDEX_2D(i % g->height, j % g->width, g->height)];
}

static inline void set_cell(GameOfLife* g, unsigned int i, unsigned int j, bool val) {
    g->grid[INDEX_2D(i % g->height, j % g->width, g->height)] = val;
}

void swap_grids_gameoflife(GameOfLife* g) {
    bool* tmp = g->current_grid;
    g->current_grid = g->grid;
    g->grid = tmp;
}

int init_gameoflife(GameOfLife* g, unsigned int width, unsigned int height, float alive_percentage) {
    g->current_grid = (bool*)malloc((width)*(height)*sizeof(bool));
    g->grid = (bool*)malloc((width)*(height)*sizeof(bool));
    if (!g->grid || !g->current_grid) {
        deinit_gameoflife(g);
        return -1;
    }

    g->is_paused = false;
    g->width = width;
    g->height = height;

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


unsigned int count_alive_neighs(GameOfLife* g, unsigned int i, unsigned int j) {
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

void step_gameoflife(GameOfLife* g) {
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
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(30);

    GameOfLife g;
    init_gameoflife(&g, 150, 150, 0.6);

    
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
        
        render_gameoflife(&g);

        EndDrawing();

        step_gameoflife(&g);
    }

    deinit_gameoflife(&g);

    CloseWindow();

    return 0;
}
