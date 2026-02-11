#include "raylib.h"

int main(){
    
    //Window dimension
    const int window_width = 720;
    const int window_height = 480;
    InitWindow(window_width, window_height, "Dashing but no dapping by Adjie Wahyudinata");

    //Rectangle dimension
    const int width{50};
    const int height{80};

    int posY{window_height - height};
    int velocity{0};

    SetTargetFPS(60);
    while (!WindowShouldClose())//! = negation operator. Negates a truth value
    {
        //Start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        
        posY += velocity;//posY = posY + velocity

        DrawRectangle(window_width/2, posY, width, height, BLACK);

        if(IsKeyPressed(KEY_SPACE)){
            velocity -= 10;
        }

        //Stop drawing
        EndDrawing();
    }
    CloseWindow(); // Close window so it doesn't leak

    return 0;
}