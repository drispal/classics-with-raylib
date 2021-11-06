/* Source.cpp
* Author: Damien-Joseph RISPAL
* Date : 24/04/2021
* Description : Main class of the project.
*/

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <utility>
#include <cmath>
#include <list>
#include <fstream>
#include <sstream>
using namespace std;

#include "piece.h"
#include "GameBoard.h"

#define MAX_INPUT_CHARS     9

int screenWidth = 1080;
int screenHeight = 800;

Vector2 RIGHT = { 1,0 };
Vector2 LEFT = { -1, 0 };
Vector2 DOWN = { 0, 1 };

int Game()
{
	// Initialization 
	int scoreInThisGame = 0;
	bool inGame = true;
	int size = 20;
	float nextTimeToMove = 0;
	float moveRate = 20;
	float nextTimeToDown = 0;
	float pieceFallRate = 10;

	Texture2D leftArrow = LoadTexture("./images/LeftArrow.png");
	Texture2D rightArrow = LoadTexture("./images/RightArrow.png");
	Texture2D downArrow = LoadTexture("./images/DownArrow.png");
	Texture2D space = LoadTexture("./images/Space.png");

	// GameBoard
	GameBoard gameBoard(300, 700, size);

	// Pieces
	list<Piece> pieces;
	list<Piece>::iterator piece;
	pieces.push_front(gameBoard.SpawnPiece());
	pieces.push_back(gameBoard.SpawnPiece());
	Rectangle boxNextPiece = {gameBoard.GetWidth()*2, gameBoard.GetShift().y*2, 6*size, 6*size};
	Vector2 positionInBox = { boxNextPiece.x + size, boxNextPiece.y + 2 * size };

	while (!WindowShouldClose() && inGame) // Game Loop
	{
		// Controls
		piece = pieces.begin();
		if (GetTime() >= nextTimeToMove)
		{
			if (IsKeyDown(KEY_LEFT) && gameBoard.PieceCanGo(LEFT, *piece))
			{
				piece->Move(LEFT, 1);
			}
			if (IsKeyDown(KEY_RIGHT) && gameBoard.PieceCanGo(RIGHT, *piece))
			{
				piece->Move(RIGHT, 1);
			}
			nextTimeToMove = GetTime() + 1 / moveRate;
		}
		if (IsKeyDown(KEY_DOWN) && gameBoard.PieceCanDown(*piece))
		{
			piece->Move(DOWN, 1);
		}
		if (IsKeyPressed(KEY_SPACE) && gameBoard.PieceCanRotate(*piece, -90))
		{
			piece->Rotate(-90);
		}

		// GameBoard Logic
		if (GetTime() >= nextTimeToDown)
		{
			// Move down the piece
			if (gameBoard.PieceCanDown(*piece))
			{
				piece->Move(DOWN, 1);
			}
			// Check if a line is full or if the party end 
			else
			{
				gameBoard.SetPiece(*piece);
				scoreInThisGame += (exp(gameBoard.LineFull()) - 1) * 10; // Update score
				if (gameBoard.GameOver())
				{
					inGame = false;
				}
				else
				{
					pieces.pop_front();
					pieces.push_back(gameBoard.SpawnPiece());
				}

			}
			nextTimeToDown = GetTime() + 1 / pieceFallRate;
		}

		// Display
		BeginDrawing();
		ClearBackground(DARKBLUE);
		if (inGame)
		{
			// GameBoard
			gameBoard.Display();

			// Pieces
			piece = pieces.begin();
			piece->Display();

			// Next Piece
			DrawRectangleRec(boxNextPiece, LIGHTGRAY);
			piece = --pieces.end();
			Piece nextPiece = *piece;
			nextPiece.Rotate(-nextPiece.GetRotation());
			nextPiece.SetPosition(positionInBox);
			nextPiece.Display();

			// Score
			DrawText("Your score :", boxNextPiece.x, boxNextPiece.y + 150, 50, RED);
			DrawText(to_string(scoreInThisGame).c_str(), boxNextPiece.x, boxNextPiece.y + 200, 50, RED);

			// Controls guide
			DrawTexture(leftArrow, boxNextPiece.x, boxNextPiece.y + 300, WHITE);
			DrawText("Move Left", boxNextPiece.x + 70, boxNextPiece.y + 330, 25, RED);
			DrawTexture(rightArrow, boxNextPiece.x, boxNextPiece.y + 400, WHITE);
			DrawText("Move Right", boxNextPiece.x + 70, boxNextPiece.y + 430, 25, RED);
			DrawTexture(downArrow, boxNextPiece.x, boxNextPiece.y + 500, WHITE);
			DrawText("Move Down", boxNextPiece.x + 70, boxNextPiece.y + 530, 25, RED);
			DrawTexture(space, boxNextPiece.x, boxNextPiece.y + 550, WHITE);
			DrawText("Rotate", boxNextPiece.x + 150, boxNextPiece.y + 600, 25, RED);

		}
		EndDrawing();
	}

	// Unload textures

	UnloadTexture(leftArrow);
	UnloadTexture(rightArrow);
	UnloadTexture(downArrow);
	UnloadTexture(space);
	return scoreInThisGame;
}

bool compare_scores(const pair<int, string> first, const pair<int, string> second)
// Method to sort the scores list.
{
	return (first.first > second.first);
}

list<pair<int, string>> getScores()
{
	ifstream scoresIn;
	scoresIn.open("./scores.txt");
	string line;
	list<pair<int, string>> scores;
	while (scoresIn >> line)
	{
		string nb, name;
		stringstream score(line);
		getline(score, nb, '|');
		getline(score, name);
		scores.push_front(make_pair(stoi(nb), name));
	}
	scoresIn.close();
	return scores;

}

void setScores(list<pair<int, string>> scores)
{
	list<pair<int, string>>::iterator score;
	score = scores.begin();
	ofstream scoresOut;
	scoresOut.open("./scores.txt");
	for (int i = 0; i < 10 && score != scores.end(); i++)
	{
		scoresOut << score->first << "|" << score->second << endl;
		score++;
	}
	scoresOut.close();
}

void Scores(list<pair<int, string>> scores)
{
	list<pair<int, string>>::iterator score;
	scores.sort(compare_scores);

	while (!WindowShouldClose()) // Scores loop
	{
		score = scores.begin();
		BeginDrawing();
		ClearBackground(DARKBLUE);
		DrawText("High Scores", (screenWidth - MeasureText("High Scores", 100)) / 2, 100, 100, RED);
		for (int i = 0; i < 10 && score != scores.end(); ++i)
		{
			string text = to_string(score->first) + " - ";
			int lengthScore = MeasureText(text.c_str(), 40);
			text = text + score->second;
			DrawText(text.c_str(), (screenWidth / 2) - lengthScore, 250 + i*40, 40, RED);
			++score;
		}
		DrawText("Press ESC to return to menu", (screenWidth - MeasureText("Press ESC to return to menu", 50)) / 2, screenHeight-100, 50, RED);
		EndDrawing();
	}

}

string GetName()
// Get the name of the player a the end of a game to add him to the scoreboard.
{
	// Variable for this scene
	char name[MAX_INPUT_CHARS + 1] = "\0";      
	int letterCount = 0;
	Rectangle textBox = { screenWidth/2 - 113, 250, 225, 50 };
	int framesCounter = 0;

	while (!WindowShouldClose() && !IsKeyPressed(KEY_ENTER))
	{
		// Get char pressed (unicode character) on the queue
		int key = GetCharPressed();

		// NOTE: Only allow keys in range [32..125]
		if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
		{
			name[letterCount] = (char)key;
			letterCount++;
		}

		if (IsKeyPressed(KEY_BACKSPACE))
		{
			letterCount--;
			if (letterCount < 0) letterCount = 0;
			name[letterCount] = '\0';
		}

		framesCounter++;

		// Draw
		// ------------------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(DARKBLUE);
		DrawText("Enter your name", (screenWidth - MeasureText("Enter your name", 100)) / 2, 100, 100, RED);
		// TextBox
		DrawRectangleRec(textBox, LIGHTGRAY);
		DrawText(name, textBox.x + 5, textBox.y + 8, 40, MAROON);
		if (letterCount < MAX_INPUT_CHARS)
		{
			// Draw blinking underscore char
			if (((framesCounter / 20) % 2) == 0) DrawText("_", textBox.x + 8 + MeasureText(name, 40), textBox.y + 12, 40, MAROON);
		}
		else DrawText("Press BACKSPACE to delete chars...", (screenWidth - MeasureText("Press BACKSPACE to delete chars...", 25)) / 2, textBox.y+60, 25, GRAY);
		
		if (letterCount == 0)
		{
			DrawText("Press ESC to return to menu", (screenWidth - MeasureText("Press ESC to return to menu", 25)) / 2, screenHeight / 2, 25, RED);
		}
		else
		{
			DrawText("Press ENTER to confirm", (screenWidth - MeasureText("Press ENTER to confirm", 25)) / 2, screenHeight / 2, 25, RED);
		}
		EndDrawing();
		// ------------------------------------------------------------------------------------------
	}
	return name;
}

void Menu()
{
	// Load scores
	list<pair<int, string>> scores = getScores();
	scores.sort(compare_scores);

	// Load logo of the menu
	Texture2D Logo = LoadTexture("./images/Logo.png");

	while (!WindowShouldClose()) // Menu loop
	{
		if (IsKeyPressed(KEY_G))
		{
			int scoreInthisGame = Game();
			string nameOfPlayer = GetName();
			if (nameOfPlayer != "")
			{
				scores.push_back(make_pair(scoreInthisGame,nameOfPlayer));
			}
		}

		if (IsKeyPressed(KEY_S)) Scores(scores);

		BeginDrawing();
		ClearBackground(DARKBLUE);

		// Logo
		DrawTexture(Logo, screenWidth / 2 - Logo.width / 2, 50, WHITE);

		// Menu
		DrawText("Press G to start a Game", (screenWidth - MeasureText("Press G to start a Game", 50)) / 2, screenHeight / 2 + 100, 50, RED);
		DrawText("Press S to show best scores", (screenWidth - MeasureText("Press G to show best scores", 50)) / 2, screenHeight / 2 + 200, 50, RED);
		DrawText("Press ESC to EXIT", (screenWidth - MeasureText("Press ESC to EXIT", 50)) / 2, screenHeight / 2 + 300, 50, RED);

		EndDrawing();
	}

	// Save the 10 best scores
	scores.sort(compare_scores);
	setScores(scores);
	UnloadTexture(Logo);

}


int main(void) 
{
	InitWindow(screenWidth, screenHeight, "Tetris");
	SetTargetFPS(60);

	Menu();

	CloseWindow();

	return 0;
}
