#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

 //Inicialização
    
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    //Criando a estrutura do personagem principal
    typedef struct {
        Vector2 pos;
        Vector2 vel;
        Vector2 acc;
        Texture2D textura_stand;
        Texture2D textura_walk_right;
        Texture2D textura_walk_left;
        Texture2D textura_jump;
        Texture2D textura_attack;
        Texture2D textura_background;
        Rectangle sourceRec_stand;
        Rectangle sourceRec_walk_right;
        Rectangle sourceRec_walk_left;
        Rectangle sourceRec_jump;
        Rectangle sourceRec_attack;
        int frameAtual;
        int altura;
        int largura;
        float scrollingBack;
        int vida;
    }MISTY;
    
    typedef struct {
        Vector2 pos;
        Vector2 vel;
        Vector2 acc;
        Texture2D texture_enemy;
        Rectangle sourceRec_enemy;
        Texture2D textura_background;
        int frameAtual;
        int altura;
        int largura;
        float scrollingBack;
    }ENEMY;
    
    void initEnemy(ENEMY* enemy) {
        enemy->pos.x = 1000;
        enemy->pos.y = 650;
        enemy->vel.y = 0;
        enemy->vel.x = 0;
        enemy->acc.y = 0.7;
        enemy->acc.x = 0;
        enemy->altura;
        enemy->largura;
        enemy->texture_enemy = LoadTexture("./assets/personagens/demon-idle.png");
        Rectangle sr_enemy = {0.0f, 0.0f, enemy->texture_enemy.width/6, enemy->texture_enemy.height};
        enemy->sourceRec_enemy = sr_enemy;
       // enemy->textura_background = LoadTexture("./background/level1/forest.png");
       // enemy->scrollingBack = 0.0f;
        enemy->frameAtual = 0;
    }
    
    void initMisty(MISTY* misty){
        misty->pos.x = 0;
        misty->pos.y = 650;
        misty->vel.y = 0;
        misty->vel.x = 0;
        misty->acc.y = 0.5;
        misty->acc.x = 0;
        misty->altura;
        misty->largura;
        misty->textura_stand = LoadTexture("./assets/WitchSprite/stand.png");
        Rectangle sr_stand = {0.0f, 0.0f, misty->textura_stand.width/4, misty->textura_stand.height};
        misty->sourceRec_stand = sr_stand;
        misty->textura_walk_right = LoadTexture("./assets/WitchSprite/walk.png");
        Rectangle sr_walk_right = {0.0f, 0.0f, misty->textura_walk_right.width/4, misty->textura_walk_right.height};
        misty->sourceRec_walk_right = sr_walk_right;
        misty->textura_walk_left = LoadTexture("./assets/WitchSprite/walk_left.png");
        Rectangle sr_walk_left = {0.0f, 0.0f, misty->textura_walk_left.width/4, misty->textura_walk_left.height};
        misty->sourceRec_walk_left = sr_walk_left;
        misty->textura_jump = LoadTexture("./assets/WitchSprite/jump.png");
        Rectangle sr_jump = {0.0f, 0.0f, misty->textura_jump.width/2, misty->textura_jump.height};
        misty->sourceRec_jump = sr_jump;
        misty->textura_attack = LoadTexture("./assets/WitchSprite/attack.png");
        Rectangle sr_attack = {0.0f, 0.0f, misty->textura_attack.width/3, misty->textura_attack.height};
        misty->sourceRec_attack = sr_attack;
        misty->textura_background = LoadTexture("./background/level1/forest.png");
        misty->scrollingBack = 0.0f;
        misty->frameAtual = 0;
        misty->vida = 3;
    }
    
    void updateEnemy(ENEMY* enemy) {
        static int frameCounter = 0;
        frameCounter++;
        
        if(frameCounter > 10) {
            frameCounter = 0;
            enemy->frameAtual++;
            
            if(enemy->frameAtual > 5) {
                enemy->frameAtual = 0;
            }
        }
        
        enemy->vel.x += enemy->acc.x;
        enemy->vel.y += enemy->acc.y;
        
        enemy->pos.x += enemy->vel.x;
        enemy->pos.y += enemy->vel.y;
    
        if(enemy->pos.x < 1000) enemy->pos.x = 1000;
        if(enemy->pos.x > 1150) enemy->pos.x = 1150;
        if(enemy->pos.y < 0) enemy->pos.y = 0;
        if(enemy->pos.y + enemy->texture_enemy.height > screenHeight) enemy->pos.y = screenHeight - enemy->texture_enemy.height;
    
    
        enemy->sourceRec_enemy.x = enemy->frameAtual*enemy->texture_enemy.width/6;
        
        
    }
    
    void updateMisty(MISTY* misty) {
        static int frameCounter = 0;
        frameCounter++;
        
        if(frameCounter > 10) {
            frameCounter = 0;
            misty->frameAtual++;
            
            if(misty->frameAtual > 5) {
                misty->frameAtual = 0;
            }
        }
        
          if (IsKeyDown(KEY_RIGHT))
        {
        misty->scrollingBack -= 0.6f;
        misty->pos.x += 1;
        misty->sourceRec_walk_right.x = misty->frameAtual*misty->textura_walk_right.width/4;
        
        if ( misty->scrollingBack <= -(misty->textura_background.width*2))  misty->scrollingBack = 0;
        
        }
        if (IsKeyDown(KEY_LEFT))
        {
        misty->scrollingBack += 0.6f;
        misty->pos.x -= 1;
        misty->sourceRec_walk_left.x = misty->frameAtual*misty->textura_walk_left.width/4;
        
        if ( misty->scrollingBack <= -(misty->textura_background.width*2))  misty->scrollingBack = 0;
        }
        if (IsKeyPressed(KEY_UP) && misty->pos.y == screenHeight - misty->textura_stand.height)
        {
        misty->vel.y = -10;
        misty->sourceRec_jump.x = misty->frameAtual*misty->textura_jump.width/2;
        }
    
        misty->vel.x += misty->acc.x;
        misty->vel.y += misty->acc.y;
        
        misty->pos.x += misty->vel.x;
        misty->pos.y += misty->vel.y;
    
        if(misty->pos.x < 0) misty->pos.x = 0;
        if(misty->pos.x > 1150) misty->pos.x = 1150;
        if(misty->pos.y < 0) misty->pos.y = 0;
        if(misty->pos.y + misty->textura_stand.height > screenHeight) misty->pos.y = screenHeight - misty->textura_stand.height;
    
    
        misty->sourceRec_walk_right.x = misty->frameAtual*misty->textura_walk_right.width/4;
        
        //Colisão
        
    }
    
    void drawEnemy(ENEMY* enemy) {
        //DrawTextureEx(enemy->textura_background, (Vector2){enemy->scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
        //DrawTextureEx(enemy->textura_background, (Vector2){enemy->textura_background.width*2 + enemy->scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
        
        DrawTextureRec(enemy->texture_enemy, enemy->sourceRec_enemy, enemy->pos, WHITE);
    }
    
    void drawMisty(MISTY* misty) {
        
        DrawTextureEx(misty->textura_background, (Vector2){misty->scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
        DrawTextureEx(misty->textura_background, (Vector2){misty->textura_background.width*2 + misty->scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
        if(IsKeyDown(KEY_RIGHT))
        {
        DrawTextureRec(misty->textura_walk_right, misty->sourceRec_walk_right, misty->pos, WHITE);
        }else if(IsKeyDown(KEY_LEFT))
        {
        DrawTextureRec(misty->textura_walk_left, misty->sourceRec_walk_left, misty->pos, WHITE);
        }else if(IsKeyDown(KEY_UP))
        {
        DrawTextureRec(misty->textura_jump, misty->sourceRec_jump, misty->pos, WHITE);
        }else
        {
        DrawTextureRec(misty->textura_stand, misty->sourceRec_stand, misty->pos, WHITE);
        }
    }


int main(void) {
    
   
    
    InitWindow(screenWidth, screenHeight, "Coven");
    SetTargetFPS(60);
    
    MISTY* misty;
    initMisty(&misty);
    
    ENEMY* enemy;
    initEnemy(&enemy);
    
    
    while (!WindowShouldClose()) {
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        updateMisty(&misty);
        drawMisty(&misty);
        updateEnemy(&enemy);
        drawEnemy(&enemy);
        
        EndDrawing();
        
    }
        
    CloseWindow();
    
    return 0;
}
