#include "raylib.h"

int main(){
    
    //Window dimension
    const int window_width = 720;
    const int window_height = 480;
    InitWindow(window_width, window_height, "Dashing but no dapping by Adjie Wahyudinata");

    //Acceleration due to gravity (pixels/sec)/second
    const int gravity{1'000};

    //texture sprite
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6; //because the sprite has 6 image
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;

    //exactly lined in the center
    Vector2 scarfyPos;
    scarfyPos.x = window_width/2 - scarfyRec.width/2;
    scarfyPos.y = window_height - scarfyRec.height;
    
    //Initial position
    int velocity{0};

    //check rectangle is in the air or not
    bool isInAir{true};
    //jump velocity (pixels/sec)
    const int jumpVelocity{-600};

    SetTargetFPS(60);
    while (!WindowShouldClose())//! = negation operator. Negates a truth value
    {
        //deltaTime (time since last frame)
        const float dT{GetFrameTime()};
        
        //Start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //Perform ground check
        if(scarfyPos.y >= window_height - scarfyRec.width){
            //rectangle is in the ground
            velocity = 0;
            isInAir = false;
        }else{
            //Apply gravity, so rectangle is in the air
            velocity += gravity * dT;
            isInAir = true;
        }

        //Movement command
        if(IsKeyPressed(KEY_SPACE) && !isInAir){//checking so no double jumping
            velocity += jumpVelocity;
        }

        //Update position
        scarfyPos.y += velocity * dT;//posY = posY + velocity * dT
        
        //Draw the object
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        //Stop drawing
        EndDrawing();
    }
    
    UnloadTexture(scarfy); //Unload it from leaking the GPU VRAM
    CloseWindow(); //Close window so it doesn't leak

    return 0;
}