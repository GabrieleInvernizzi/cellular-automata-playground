#include "raylib.h"

#include "gameoflife.h"


#define SCREEN_WIDTH (500)
#define SCREEN_HEIGHT (500)
#define WINDOW_TITLE "Cellular Automata Explorer"


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
