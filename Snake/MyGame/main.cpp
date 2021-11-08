/*
*######### SNAKE #########
* File : main.cpp
* Description : Just the simple snake game we like.
* Date : April 2021
* Author : Damien-Joseph RISPAL
*/

#include <raylib.h>
#include <raymath.h>
#include <list>
#include <iostream>
#include <cstdlib>
using namespace std;

// Overloading
//--------------------------------------------------------------------------------------

bool operator!=(Vector2 vector1, Vector2 vector2)
{
    return vector1.x != vector2.x || vector1.y != vector2.y;
}

//--------------------------------------------------------------------------------------


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 720;
    const int screenHeight = 480;
        
    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);    

    // GameOver Variables
    bool isEndOfGame = false;
    int textSize = 20;
    const char* gameOverText = "GameOver";
    int gameOverTextLength = MeasureText(gameOverText, 20);
    const char* restartText = "Click Here to Restart";
    int restartTextLength = MeasureText(restartText, 20);
    Rectangle restartBtnBounds =
    {
        ((screenWidth - restartTextLength) / 2),
        ((screenHeight / 2) + 40),
        restartTextLength,
        textSize
    };

    // Controls variables
    int snakeSpeed = 7;
    const Vector2 dRight{ snakeSpeed,0 };
    const Vector2 dLeft{ -snakeSpeed,0 };
    const Vector2 dDown{ 0,snakeSpeed };
    const Vector2 dUp{ 0,-snakeSpeed };
    Vector2 snakeDirection = dRight;


    // Mouse controls
    Vector2 mousePoint{ 0.0f, 0.0f };

    // Snake snakeSize
    Vector2 snakePosition{ 0,0 };
    list<Vector2> snakePoints = { snakePosition };
    Vector2 snakeSize{ 25,25 };

    // PowerUp variables
    float powerUpSize = 9.0;
    Vector2 powerUpPosition{ rand()%screenWidth, rand()%screenHeight};
    bool collision = false;

    //--------------------------------------------------------------------------------------

    // Main game loop each frames
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        // Mouse position
        mousePoint = GetMousePosition();

        // Snake Controls 
        if (IsKeyDown(KEY_RIGHT) && snakeDirection != dLeft)
        {
            snakeDirection = dRight;
        }
        else if (IsKeyDown(KEY_LEFT) && snakeDirection != dRight)
        {
            snakeDirection = dLeft;
        }
        else if (IsKeyDown(KEY_UP) && snakeDirection != dDown)
        {
            snakeDirection = dUp;
        }
        else if (IsKeyDown(KEY_DOWN) && snakeDirection != dUp)
        {
            snakeDirection = dDown;
        }

        // Update of the snake
        snakePosition = Vector2Add(snakePosition, snakeDirection);
        collision = CheckCollisionCircleRec(powerUpPosition, powerUpSize, Rectangle{snakePosition.x,snakePosition.y,snakeSize.x,snakeSize.y});
        
        // GameOver
        for (Vector2 vec : snakePoints)
        {
            if (vec.x == snakePosition.x && vec.y == snakePosition.y)
            {
                isEndOfGame = true;
            }
        }

        //Restart
        if (isEndOfGame && CheckCollisionPointRec(mousePoint, restartBtnBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            isEndOfGame = false;
            snakePosition = { 0,0 };
            snakePoints.clear();
            snakePoints.push_front(snakePosition);
            snakeDirection = dRight;
        }

        // Snake Drawing
        snakePoints.push_front(snakePosition);
        if (!collision)
        {
            snakePoints.pop_back();
        }

        // PowerUp
        if (collision)
        {
            powerUpPosition.x = rand() % screenWidth;
            powerUpPosition.y = rand() % screenHeight;
        }

        // Borders
        if (snakePosition.x > screenWidth-25) snakePosition.x = 0;
        if (snakePosition.y > screenHeight-25) snakePosition.y = 0;
        if (snakePosition.x < 0) snakePosition.x = screenWidth - 25;
        if (snakePosition.y < 0) snakePosition.y = screenHeight - 25;
        

        // Affichage
        BeginDrawing();

        ClearBackground(BLACK);

        if (!isEndOfGame)
        {
            // Power Up
            DrawCircleV(powerUpPosition, powerUpSize, GREEN);

            // Snake
            for (Vector2 vec : snakePoints)
            {
                DrawRectangleV(vec, snakeSize, RED);
            }
        }
        else
        {
            // Text
            DrawText(gameOverText, ((screenWidth-gameOverTextLength)/2), (screenHeight/2), textSize, RED);
            DrawText(restartText, ((screenWidth-restartTextLength)/ 2), ((screenHeight/2) + 40), textSize, RED);
        }

        EndDrawing();

    }
    //--------------------------------------------------------------------------------------

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}