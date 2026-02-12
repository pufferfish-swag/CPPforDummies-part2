#include "raylib.h"

int main(){
    
    //Window dimension
    const int window_width = 720;
    const int window_height = 480;
    InitWindow(window_width, window_height, "Dashing but no dapping by Adjie Wahyudinata");

    //Acceleration due to gravity (pixels/frame)/frame
    const int gravity{1};

    //Rectangle dimension
    const int width{50};
    const int height{80};
    
    //Initial position
    int posY{window_height - height};
    int velocity{0};

    //check rectangle is in the air or not
    bool isInAir{true};
    const int jumpVelocity{-20};

    SetTargetFPS(60);
    while (!WindowShouldClose())//! = negation operator. Negates a truth value
    {
        //Start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        
        //Perform ground check
        if(posY >= window_height - height){
            //rectangle is in the ground
            velocity = 0;
            isInAir = false;
        }else{
            //Apply gravity, so rectangle is in the air
            velocity += gravity;
            isInAir = true;
        }

        //Movement command
        if(IsKeyPressed(KEY_SPACE) && !isInAir){//checking so no double jumping
            velocity += jumpVelocity;
        }

        //Update position
        posY += velocity;//posY = posY + velocity
        
        //Draw the object
        DrawRectangle(window_width/2, posY, width, height, BLACK);

        //Stop drawing
        EndDrawing();
    }
    CloseWindow(); //Close window so it doesn't leak

    return 0;
}