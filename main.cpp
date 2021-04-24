#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
using namespace std;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;  // FullScreen
    bool isInGame = false;

    InitWindow(screenWidth, screenHeight, "Pong");
    SetTargetFPS(60);

    // Definitions of players
    int playerWidth = 300;
    int playerHeight = 50;
    Rectangle player1 = { 0,0,playerHeight,playerWidth };
    Rectangle player2 = { screenWidth-playerHeight,0,playerHeight,playerWidth };
    int playerSpeed = 0;

    // Definition of the ball
    int ballSize = 20;
    int ballSpeed = 0;
    Vector2 ballPosition = { (screenWidth / 2),(screenHeight / 2) };
    Vector2 ballDirection = {(rand()%2),(rand()%2)};

    // Menu
    int textSize = 30;
    const char* menuText = "Press Bar Space To Launch The Ball";
    int textLength = MeasureText(menuText, textSize);


    // Main game loop each frames
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // GameStart
        if (!isInGame && IsKeyPressed(KEY_SPACE))
        {
            isInGame = true;
            ballSpeed = 7;
            playerSpeed = 15;
        }

        // Controls
        // Player1
        if (IsKeyDown(KEY_S) && player1.y != screenHeight-playerWidth)
        {
            player1.y += playerSpeed;
        }
        else if (IsKeyDown(KEY_W) && player1.y != 0)
        {
            player1.y -= playerSpeed;
        }
        // Player2
        if (IsKeyDown(KEY_DOWN) && player2.y != screenHeight - playerWidth)
        {
            player2.y += playerSpeed;
        }
        else if (IsKeyDown(KEY_UP) && player2.y != 0)
        {
            player2.y -= playerSpeed;
        }

        // Ball movements
        if (ballPosition.y <= ballSize || ballPosition.y >= screenHeight-ballSize)
        {
            ballDirection.y = -ballDirection.y;
        }
        if (CheckCollisionCircleRec(ballPosition, ballSize, player1) || CheckCollisionCircleRec(ballPosition, ballSize, player2))
        {
            ballDirection.x = -ballDirection.x;
        }
        ballPosition = Vector2Add(ballPosition, Vector2Scale(ballDirection,ballSpeed));

        // GameOver
        if (ballPosition.x <= 0 || ballPosition.x >= screenWidth)
        {
            isInGame = false;
            ballSpeed = 0;
            playerSpeed = 0;
            ballPosition = { (screenWidth / 2),(screenHeight / 2) };
        }

        // Display
        BeginDrawing();

        ClearBackground(BLACK);
        if (isInGame)
        {
            DrawCircleV(ballPosition, ballSize, RED);
        }
        else
        {
            DrawText(menuText, ((screenWidth-textLength)/2), (screenHeight / 2), textSize, WHITE);
        }
        
        DrawRectangleRec(player1, WHITE);
        DrawRectangleRec(player2, WHITE);

        EndDrawing();

    }
    //--------------------------------------------------------------------------------------

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}