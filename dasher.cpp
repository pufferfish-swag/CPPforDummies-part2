#include "raylib.h"

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
    Rectangle nebRec{0.0, 0.0, nebula.width/divide, nebula.height/divide};
    Vector2 nebPos{window_width, window_height - nebRec.height};

    Rectangle neb2Rec{0.0, 0.0, nebula.width/divide, nebula.height/divide};
    Vector2 neb2Pos{window_width + 300, window_height - nebRec.height};

    //animation frame of the nebula
    int nebFrame{};
    const float nebUdateTime{1.0/12.0};
    float nebRunningTime{};

    int neb2Frame{};
    const float neb2UpdateTime{1.0/16.0};
    float neb2RunningTime;

    //nebula X velocity (pixels/sec)
    int nebVel{-200};

    //texture & variable of scarfy (character)
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6; //because the sprite has 6 image
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    //scarfy position in the center
    Vector2 scarfyPos;
    scarfyPos.x = window_width/2 - scarfyRec.width/2;
    scarfyPos.y = window_height - scarfyRec.height;

    //animation frame
    int frame{};

    //amount of time before we update the animation frame
    const float updateTime{1.0/12.0};
    float runningTime{};

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

        //update nebula position
        nebPos.x += nebVel * dT;
        neb2Pos.x += nebVel * dT;

        //Update position
        scarfyPos.y += velocity * dT;//posY = posY + velocity * dT

        //Check if scarfy jump or not. if it is, froze the animation.
        if(!isInAir){
        //Update running animation time
            runningTime += dT;
            if(runningTime >= updateTime){
                runningTime = 0.0;
                //Update animation frame
                scarfyRec.x = frame * scarfyRec.width;
                frame++;
                if(frame > 5){
                frame = 0;
                }
            }
        }

        //Update nebula animation frame
        nebRunningTime += dT;
        if(nebRunningTime >= nebUdateTime){
            nebRunningTime = 0.0;
            //Update anim frame
            nebRec.x = nebFrame * nebRec.width;
            nebFrame++;
            if(nebFrame > 7){
                nebFrame = 0;
            }
        }

        //Update 2nd nebula animation frame
        neb2RunningTime += dT;
        if(neb2RunningTime >= neb2UpdateTime){
            neb2RunningTime = 0.0;
            //Update anim frame
            neb2Rec.x = neb2Frame * neb2Rec.width;
            neb2Frame++;
            if(neb2Frame > 7){
                neb2Frame = 0;
            }
        }

        //Draw scarfy
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);
        //Draw nebula
        DrawTextureRec(nebula, nebRec, nebPos, WHITE);
        DrawTextureRec(nebula, neb2Rec, neb2Pos, RED);

        //Stop drawing
        EndDrawing();
    }
    
    //Unload it from leaking the GPU VRAM
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();

    return 0;
}