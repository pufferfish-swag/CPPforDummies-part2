#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


int main(){
    
    //Window dimension
    const int window_width = 720;
    const int window_height = 480;
    //Initialize window
    InitWindow(window_width, window_height, "Dashing but no dapping by Adjie Wahyudinata");

    //Acceleration due to gravity (pixels/sec)/second
    const int gravity{1'000};

    //I do this because it anooys me with the yellow underline if i do it manual lol
    const float divide = 8;

    //texture & variable of nebula (enemy)
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    //AnimData for nebula
    AnimData nebData{
        {0.0, 0.0, nebula.width/divide, nebula.height/divide}, //Rectangle rec
        {window_width, window_height - nebula.height/divide}, //Vector2 pos
        0, //int frame
        1.0/12.0, //float updateTime
        0 //float runningTime
    };

    AnimData neb2Data{
        {0.0, 0.0, nebula.width/divide, nebula.height/divide}, //Rectangle rec
        {window_width + 300, window_height - nebula.height/divide}, //Vector2 pos
        0, //int frame
        1.0/16.0, //float updateTime
        0 //float runningTime
    };

    //nebula X velocity (pixels/sec)
    int nebVel{-200};

    //texture & variable of scarfy (character)
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    //AnimData for scarfy
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;

    scarfyData.pos.x = window_width/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = window_height - scarfyData.rec.height;

    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;
    //check rectangle is in the air or not
    bool isInAir{};
    //jump velocity (pixels/sec)
    const int jumpVelocity{-600};

    //Initial position
    int velocity{0};

    SetTargetFPS(60);
    while (!WindowShouldClose())//! = negation operator. Negates a truth value
    {
        //deltaTime (time since last frame)
        const float dT{GetFrameTime()};
        
        //Start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //Set movement to scarfy, perform ground check
        if(scarfyData.pos.y >= window_height - scarfyData.rec.height){
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

        //update nebula position
        nebData.pos.x += nebVel * dT;
        neb2Data.pos.x += nebVel * dT;

        //Update position
        scarfyData.pos.y += velocity * dT;//posY = posY + velocity * dT

        //Check if scarfy jump or not. if it is, froze the animation.
        if(!isInAir){
        //Update running animation time
            scarfyData.runningTime += dT;
            if(scarfyData.runningTime >= scarfyData.updateTime){
                scarfyData.runningTime = 0.0;
                //Update animation frame
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                scarfyData.frame++;
                if(scarfyData.frame > 5){
                scarfyData.frame = 0;
                }
            }
        }

        //Update nebula animation frame
        nebData.runningTime += dT;
        if(nebData.runningTime >= nebData.updateTime){
            nebData.runningTime = 0.0;
            //Update anim frame
            nebData.rec.x = nebData.frame * nebData.rec.width;
            nebData.frame++;
            if(nebData.frame > 7){
                nebData.frame = 0;
            }
        }

        //Update 2nd nebula animation frame
        neb2Data.runningTime += dT;
        if(neb2Data.runningTime >= neb2Data.updateTime){
            neb2Data.runningTime = 0.0;
            //Update anim frame
            neb2Data.rec.x = neb2Data.frame * neb2Data.rec.width;
            neb2Data.frame++;
            if(neb2Data.frame > 7){
                neb2Data.frame = 0;
            }
        }

        //Draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        //Draw nebula
        DrawTextureRec(nebula, nebData.rec, nebData.pos, WHITE);
        DrawTextureRec(nebula, neb2Data.rec, neb2Data.pos, RED);

        //Stop drawing
        EndDrawing();
    }
    
    //Unload it from leaking the GPU VRAM
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();

    return 0;
}