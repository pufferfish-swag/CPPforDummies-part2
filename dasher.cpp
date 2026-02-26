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
bool isOnGround(const AnimData& data, int windowHeight)//& (just one) means reference so no need to copying more than it should
{
    return data.pos.y >= windowHeight - data.rec.height;
}

//well, update animation data
void updateAnimData(AnimData& data, float deltaTime, int maxFrame){
    //update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime){
        data.runningTime = 0.0f;
        //update animation frame
        data.rec.x  = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > maxFrame){
            data.frame = 0;
        }
    }
}

// Draw the parallax background layers
void drawBackground(Texture2D background, Texture2D midground, Texture2D foreground,
    float bgX, float mgX, float fgX){

        const float bgScale{3.0};

        //Draw backgorund
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, bgScale, WHITE);
        Vector2 bg2Pos{bgX + background.width * 3, 0.0}; //bg width * 3 because bgScale is 3.
        DrawTextureEx(background, bg2Pos, 0.0, bgScale, WHITE);
        
        //Draw midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, bgScale, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 3, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, bgScale, WHITE);
        
        //Draw foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, bgScale, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 3, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, bgScale, WHITE);
        
}

//Update background scroll position
void updateBackgroundScroll(float& bgX, float& mgX, float& fgX, float dT,
    Texture2D bg, Texture2D mg, Texture2D fg){
    
    //Parallax scrolling
    const float backgroundScrollSpeed{20};
    const float midgroundScrollSpeed{40};
    const float foregroundScrollSpeed{80};

    bgX -= backgroundScrollSpeed * dT;
    if (bgX <= -bg.width * 3) bgX = 0.0f;
    
    mgX -= midgroundScrollSpeed * dT;
    if (mgX <= -mg.width * 3) mgX = 0.0f;
    
    fgX -= foregroundScrollSpeed * dT;
    if (fgX <= -fg.width * 3) fgX = 0.0f;   
}

int main(){

    const int WindowWidth {768};
    const int WindowHeight {570};

    //Initialize window
    InitWindow(WindowWidth, WindowHeight, "Dashing but no dapping by Adjie Wahyudinata");

    //Acceleration due to gravity (pixels/sec)/second
    const float gravity{1000.0};

    //I do this because cpp annoys me with the yellow underline if I type 8 manually lol
    const float divide = 8;

    //texture & variable of nebula (enemy)
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    
    //AnimData for nebula
    const int sizeOfNebulaEnemy{10};
    AnimData nebulaEnemy[sizeOfNebulaEnemy]{};

    const int nebulaSpacing{300};

    for (int i = 0; i < sizeOfNebulaEnemy; i++){
        nebulaEnemy[i].rec.x = 0.0;
        nebulaEnemy[i].rec.y = 0.0;
        nebulaEnemy[i].rec.width = nebula.width/divide;
        nebulaEnemy[i].rec.height = nebula.height/divide;
        nebulaEnemy[i].pos.y = WindowHeight - nebula.height/divide;
        nebulaEnemy[i].pos.x = WindowWidth + i * nebulaSpacing; //multiplying the nebula enemy and giving them space
        nebulaEnemy[i].frame = 0;
        nebulaEnemy[i].runningTime = 0.0;
        nebulaEnemy[i].updateTime = 1.0/16.0;
    }
    //well.. finish line
    float finishLine{nebulaEnemy[sizeOfNebulaEnemy - 1].pos.x};

    //nebula X velocity (pixels/sec)
    float nebulaVelocity{-200.0};

    //texture & variable of scarfy (character)
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    //AnimData for scarfy
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;

    scarfyData.pos.x = WindowWidth/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = WindowHeight - scarfyData.rec.height;

    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;
    //check rectangle is in the air or not
    bool isInAir{};
    //jump velocity (pixels/sec)
    const float jumpVelocity{-600.0};

    //Initial position
    float velocity{0.0};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};//Because the number will be filled later in while function
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    //game over checker
    bool collision{};

    SetTargetFPS(60);
    while (!WindowShouldClose())//! = negation operator. Negates a truth value
    {
        //deltaTime (time since last frame)
        const float dT = GetFrameTime();
        
        updateBackgroundScroll(bgX, mgX, fgX, dT, background, midground, foreground);

        //Start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        drawBackground(background, midground, foreground, bgX, mgX, fgX);

        //perform ground check
        if(isOnGround(scarfyData, WindowHeight)){
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
            nebulaEnemy[i].pos.x += nebulaVelocity * dT;
        }

        //update finishLine
        finishLine += nebulaVelocity * dT;

        //Update position
        scarfyData.pos.y += velocity * dT;//posY = posY + velocity * dT

        const int maxFrameScarfy{5};
        const int maxFrameNebulaEnemy{7};
        

        //Check if scarfy jump or not. if it is, froze the animation.
        if(!isInAir){
            updateAnimData(scarfyData, dT, maxFrameScarfy);
        }

        for (int i = 0; i < sizeOfNebulaEnemy; i++ ){
            updateAnimData(nebulaEnemy[i], dT, maxFrameNebulaEnemy);
        }

        for (const AnimData& nebula : nebulaEnemy){//Read only, not modify
            
            //Collision padding
            float collisionPad{50};
            
            Rectangle nebRec{
                nebula.pos.x + collisionPad,
                nebula.pos.y + collisionPad,
                nebula.rec.width - 2*collisionPad,
                nebula.rec.height - 2*collisionPad
            };
            
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            
            if (CheckCollisionRecs(nebRec, scarfyRec)){
                collision = true;
            }
        }

        if(collision){
            
            // lose the game dawg
            DrawText("Game Over!", WindowWidth/4, WindowHeight/2, 50, MAROON);
        
        }else if(scarfyData.pos.x >= finishLine){
            
            // win the game dawg
            DrawText("You Win!", WindowWidth/4, WindowHeight/2, 50, WHITE);

        }else{
            
            for(int i = 0; i < sizeOfNebulaEnemy; i++){
                //Draw nebula
                DrawTextureRec(nebula, nebulaEnemy[i].rec, nebulaEnemy[i].pos, WHITE);
            }

            //Draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        
        }
        
        //Stop drawing
        EndDrawing();
    }
    
    //Unload it from leaking the GPU VRAM
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    
    CloseWindow();

    return 0;

}
