/* 
* || space_invader.cpp ||
* Author : Damien-Joseph Rispal
* Date : 26/04/2021
*/

#include <raylib.h>
#include <list>
#include <utility>
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

bool playerHited(Vector2 missilePosition, Vector2 missileSize, Vector2 playerPosiiton, Vector2 playerShape)
// Check if the player is hit by a enemy missile.
{
	Rectangle player = { playerPosiiton.x, playerPosiiton.y, playerShape.x, playerShape.y };
	Rectangle missile = { missilePosition.x, missilePosition.y, missileSize.x, missileSize.y};
	return CheckCollisionRecs(player, missile);
}

int invaderHited(Vector2 missileP, Vector2 missileSize, pair <Vector2, bool> invaders[], Vector2 invaderSize, int nbOfInvaders)
// Return the Index of the hited invader, -1 if no hit
{
	Rectangle missile = { missileP.x, missileP.y, missileSize.x, missileSize.y };
	for (int i = 0; i < nbOfInvaders; ++i)
	{
		Rectangle invader = { invaders[i].first.x,invaders[i].first.y, invaderSize.x, invaderSize.y };
		if (invaders[i].second && CheckCollisionRecs(missile, invader))
		{
			return i;
		}
	}

	return -1;
}

Vector2 playerShoot(Vector2 _shipP, int missileLength)
// Return a new missile shot by the player
{
	Vector2 missile = { _shipP.x, _shipP.y - missileLength };
	return missile;
}

Vector2 invaderShoot(Vector2 shooterP, int missileLength)
// Return a new missile shot by an ennemy
{
	Vector2 missile = {shooterP.x, shooterP.y + missileLength};
	return missile;
}

int main(void)
{
	// Initialization
	int screenWidth = 1080;
	int screenHeight = 720;
	bool endOfGame = false;
	int gameLevel = 0;

	InitWindow(screenWidth,screenHeight, "Space Invaders");
	SetTargetFPS(60);

	// Spacechip (movement, shoot, shape)
	int playerLives = 3;
	bool playerIsHited = false;
		// Shape
	Texture2D shipTexture = LoadTexture("./SpaceShip.png");
	Vector2 shipShape = { shipTexture.width, shipTexture.height };
		// Movement
	int shipSpeed = 1000;
	Vector2 shipPosition = { (screenWidth / 2), screenHeight - shipShape.y };
		// Shoot
	int missileSpeed = 700;
	Vector2 missileShape = {10,10};
	list<Vector2> missiles = {};
	float nextTimeToShoot = 0;
	float shootingRate = 2;

	// Invaders (movement, shoot, shape)
	const int invadersNumber = 50;
	const int invaderLine = 10; // Number of invaders in a row
	int invadersAlives = invadersNumber;
		// Shape
	Texture2D invaderTexture = LoadTexture("./Invader.png");
	Vector2 invaderShape = { invaderTexture.width, invaderTexture.height };
	Vector2 invaderGap = { invaderTexture.width/2, invaderTexture.height/4 };
	pair <Vector2, bool> invaders[invadersNumber];
		// Movement
	int invaderSpeed = 125;
	bool descending = false;
	int direction = 1;
	for (int i = 0; i < invadersNumber; ++i)
	{
		invaders[i].first.x = invaderGap.x + (invaderShape.x + invaderGap.x) * (i%invaderLine);
		invaders[i].first.y = (i/invaderLine)*(invaderShape.y+ invaderGap.y);
		invaders[i].second = true;
	}
		// Shoot
	int shooter = rand() % invadersNumber;
	float invaderShootingRate = 3;
	float nextInvaderShoot = 0;
	int invaderMissileSpeed = 700;
	list<Vector2> invadersMissiles = {};

	// Utility
	list<Vector2>::iterator it;

	// Main loop
	while (!WindowShouldClose()) // Red cross or ESC KEY (by default)
	{
		// Ship movement
		if (IsKeyDown(KEY_RIGHT) && shipPosition.x < screenWidth-shipShape.x-100) shipPosition.x += shipSpeed * GetFrameTime();
		if (IsKeyDown(KEY_LEFT) && shipPosition.x > 100) shipPosition.x -= shipSpeed * GetFrameTime();

		// Invaders movement
		if ((invaders[invaderLine - 1].first.x > screenWidth - invaderGap.x - invaderShape.x)
			|| (invaders[0].first.x < invaderGap.x))
		{
			descending = true;
			direction = -direction;
		}
		for (int i = 0; i < invadersNumber; ++i)
		{
			invaders[i].first.x += invaderSpeed * direction * GetFrameTime();
			if (descending) invaders[i].first.y += invaderGap.y;
		}
		descending = false;

		// Player Missiles
		if (IsKeyDown(KEY_SPACE) && GetTime() >= nextTimeToShoot)
		{
			missiles.push_front(playerShoot(shipPosition, missileShape.y));
			nextTimeToShoot = GetTime() + 1 / shootingRate;
		}
		for (it = missiles.begin(); it != missiles.end();)
		{
			int hit = invaderHited(*it, missileShape, invaders, invaderShape, invadersNumber);
			if (it->y > 0)
			{
				// Movement
				it->y -= missileSpeed*GetFrameTime();
				
				// Invaders hit
				if (hit >= 0)
				{
					it = missiles.erase(it);
					if (--invadersAlives == 0)
					{
						endOfGame = true;
					}
					else
					{
						invaders[hit].second = false;
					}
				}
				else
				{
					++it;
				}
			}
			// Borders
			else
			{
				it = missiles.erase(it);

			}
		}

		// Invaders missiles
		if (GetTime() >= nextInvaderShoot)
		{
			shooter = rand() % invadersNumber;
			while (!invaders[shooter].second)
			{
				shooter = rand() % invadersNumber;
			}
			invadersMissiles.push_front(invaderShoot(invaders[shooter].first, missileShape.y));
			nextInvaderShoot = GetTime() + 1 / invaderShootingRate;
		}
		for (it = invadersMissiles.begin(); it != invadersMissiles.end();)
		{
			if (it->y < screenHeight)
			{
				it->y += invaderMissileSpeed * GetFrameTime();

				if (playerHited(*it, missileShape, shipPosition, shipShape))
				{
					it = invadersMissiles.erase(it);
					playerLives -= 1;
					playerIsHited = true;
				}
				else
				{
					++it;
				}
			} 
			// Borders
			else
			{
				it = invadersMissiles.erase(it);

			}
		}

		// EndGame (if player hasn't lives or invaders number = 0)
		// Improvement = end of the game when an invader reach the bottom of the screen.

		if (playerLives == 0 || endOfGame)
		{
			// Spaceship
			playerLives = 3;
			shipPosition.x = (screenWidth / 2); 
			shipPosition.y = (screenHeight - shipShape.y);
			missiles.clear();
			// Invaders
			invadersAlives = invadersNumber;
			descending = false;
			direction = 1;
			for (int i = 0; i < invadersNumber; ++i)
			{
				invaders[i].first.x = invaderGap.x + (invaderShape.x + invaderGap.x) * (i % invaderLine);
				invaders[i].first.y = (i / invaderLine) * (invaderShape.y + invaderGap.y);
				invaders[i].second = true;
			}
			invadersMissiles.clear();
			// Game
			if (endOfGame)
			{
				invaderShootingRate += 0.5;
				invaderMissileSpeed += 10;
				gameLevel++;
			}
			else
			{
				invaderShootingRate = 3;
				invaderMissileSpeed = 700;
				gameLevel = 0;
			}
			endOfGame = false;
		}


		// Display
		BeginDrawing();
		
		ClearBackground(BLACK);
		if (playerIsHited) ClearBackground(GRAY);
			
			// Level
		DrawText(to_string(gameLevel).c_str(), screenWidth - 10, 1, 20, WHITE);
		DrawText("Level: ", screenWidth - 20 - MeasureText("Level:", 20), 1, 20, WHITE);

			// Lives
		DrawText(to_string(playerLives).c_str(), 1, 1, 20, WHITE);
		DrawText(" lives remaining", 10, 1, 20, WHITE);

			// Ship
		DrawTextureV(shipTexture, shipPosition, WHITE);

			// Invaders
		for (int i = 0; i < invadersNumber; ++i)
		{
			if(invaders[i].second) DrawTextureV(invaderTexture, invaders[i].first, GREEN);
		}
			
			// Missiles
		for (it = missiles.begin(); it != missiles.end(); ++it)
		{
			DrawRectangleV(*it, missileShape, RED);
		}
		for (it = invadersMissiles.begin(); it != invadersMissiles.end(); ++it)
		{
			DrawRectangleV(*it, missileShape, GREEN);
		}

		EndDrawing();

		playerIsHited = false;

	}

	// De-Initialization
	UnloadTexture(invaderTexture);
	UnloadTexture(shipTexture);
	CloseWindow();
}