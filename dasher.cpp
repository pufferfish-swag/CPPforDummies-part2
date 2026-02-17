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
    
    //array with dimension
    int windowDimension[2];
    windowDimension[0] = 720; //width
    windowDimension[1] = 480; //height

    //Initialize window
    InitWindow(windowDimension[0], windowDimension[1], "Dashing but no dapping by Adjie Wahyudinata");

    //Acceleration due to gravity (pixels/sec)/second
    const int gravity{1'000};

    //I do this because cpp annoys me with the yellow underline if I type 8 manually lol
    const float divide = 8;

    //texture & variable of nebula (enemy)
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    //AnimData for nebula
    AnimData nebData{
        {0.0, 0.0, nebula.width/divide, nebula.height/divide}, //Rectangle rec
        {windowDimension[0], windowDimension[1] - nebula.height/divide}, //Vector2 pos
        0, //int frame
        1.0/12.0, //float updateTime
        0 //float runningTime
    };

    AnimData neb2Data{
        {0.0, 0.0, nebula.width/divide, nebula.height/divide}, //Rectangle rec
        {windowDimension[0] + 300, windowDimension[1] - nebula.height/divide}, //Vector2 pos
        0, //int frame
        1.0/16.0, //float updateTime
        0 //float runningTime
    };

    AnimData nebulaEnemy[2]{nebData, neb2Data};

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

    scarfyData.pos.x = windowDimension[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimension[1] - scarfyData.rec.height;

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
        if(scarfyData.pos.y >= windowDimension[1] - scarfyData.rec.height){
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
        nebulaEnemy[0].pos.x += nebVel * dT;
        nebulaEnemy[1].pos.x += nebVel * dT;

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
        nebulaEnemy[0].runningTime += dT;
        if(nebulaEnemy[0].runningTime >= nebulaEnemy[0].updateTime){
            nebulaEnemy[0].runningTime = 0.0;
            //Update anim frame
            nebulaEnemy[0].rec.x = nebulaEnemy[0].frame * nebulaEnemy[0].rec.width;
            nebulaEnemy[0].frame++;
            if(nebulaEnemy[0].frame > 7){
                nebulaEnemy[0].frame = 0;
            }
        }

        //Update 2nd nebula animation frame
        nebulaEnemy[1].runningTime += dT;
        if(nebulaEnemy[1].runningTime >= nebulaEnemy[1].updateTime){
            nebulaEnemy[1].runningTime = 0.0;
            //Update anim frame
            nebulaEnemy[1].rec.x = nebulaEnemy[1].frame * nebulaEnemy[1].rec.width;
            nebulaEnemy[1].frame++;
            if(nebulaEnemy[1].frame > 7){
                nebulaEnemy[1].frame = 0;
            }
        }

        //Draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        //Draw nebula
        DrawTextureRec(nebula, nebulaEnemy[0].rec, nebulaEnemy[0].pos, WHITE);
        DrawTextureRec(nebula, nebulaEnemy[1].rec, nebulaEnemy[1].pos, RED);

        //Stop drawing
        EndDrawing();
    }
    
    //Unload it from leaking the GPU VRAM
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();

    return 0;
}