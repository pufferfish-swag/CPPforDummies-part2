#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

//refactor ground check
bool isOnGround(AnimData data, int windowHeight){
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame){
    //update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime){
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x  = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > maxFrame){
            data.frame = 0;
        }
    }
    return data;
}

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
    const int sizeOfNebulaEnemy{10};
    AnimData nebulaEnemy[sizeOfNebulaEnemy]{};

    for (int i = 0; i < sizeOfNebulaEnemy; i++){
        nebulaEnemy[i].rec.x = 0.0;
        nebulaEnemy[i].rec.y = 0.0;
        nebulaEnemy[i].rec.width = nebula.width/divide;
        nebulaEnemy[i].rec.height = nebula.height/divide;
        nebulaEnemy[i].pos.y = windowDimension[1] - nebula.height/divide;
        nebulaEnemy[i].pos.x = windowDimension[0] + i * 300; //multiplying teh nebula enemy
        nebulaEnemy[i].frame = 0;
        nebulaEnemy[i].runningTime = 0.0;
        nebulaEnemy[i].updateTime = 1.0/16.0;
    }

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
        if(isOnGround(scarfyData, windowDimension[1])){
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

        for(int i = 0; i < sizeOfNebulaEnemy; i++){
            //Update position of each nebula
            nebulaEnemy[i].pos.x += nebVel * dT;
        }

        //Update position
        scarfyData.pos.y += velocity * dT;//posY = posY + velocity * dT

        //Check if scarfy jump or not. if it is, froze the animation.
        if(!isInAir){
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for (int i = 0; i < sizeOfNebulaEnemy; i++ ){
            nebulaEnemy[i].runningTime += dT;
            if(nebulaEnemy[i].runningTime >= nebulaEnemy[i].updateTime){
                nebulaEnemy[i].runningTime = 0.0;
                //Update anim frame
                nebulaEnemy[i].rec.x = nebulaEnemy[i].frame * nebulaEnemy[i].rec.width;
                nebulaEnemy[i].frame++;
                if(nebulaEnemy[i].frame > 7){
                    nebulaEnemy[i].frame = 0;
                }
            }
        }

        for(int i = 0; i < sizeOfNebulaEnemy; i++){
            //Draw nebula
            DrawTextureRec(nebula, nebulaEnemy[i].rec, nebulaEnemy[i].pos, WHITE);
        }

        //Draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        
        //Stop drawing
        EndDrawing();
    }
    
    //Unload it from leaking the GPU VRAM
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();

    return 0;

}
