/* piece.cpp
* Author: Damien-Joseph RISPAL
* Date : 24/04/2021
* Description : Declaration of the class Piece which implement all kind of Tetris pieces.
*/

#include "piece.h"

#include <raymath.h>
#include <cstdlib>
#include <cmath>
using namespace std;

// CONSTRUCTOR

Piece::Piece(int _blockSize, Shape _shape, Color _color)
{
	color = _color;
	shape = _shape;
	blockSize = _blockSize;
	rotation = 0;
	// Instanciate the piece at the coordinates (0,0)
	position[0] = Vector2{ 0,0 };
	switch (shape)
	{
	case I_BLOCK:
		position[1].x = blockSize;
		position[1].y = 0;
		position[2].x = 2*blockSize;
		position[2].y = 0;
		position[3].x = 3*blockSize;
		position[3].y = 0;
		break;
	case J_BLOCK:
		position[1].x = 0;
		position[1].y = blockSize;
		position[2].x = blockSize;
		position[2].y = blockSize;
		position[3].x = 2 * blockSize;
		position[3].y = blockSize;
		break;
	case L_BLOCK:
		position[1].x = blockSize;
		position[1].y = 0;
		position[2].x = 2 * blockSize;
		position[2].y = 0;
		position[3].x = 2 * blockSize;
		position[3].y = -blockSize;
		break;
	case O_BLOCK:
		position[1].x = blockSize;
		position[1].y = 0;
		position[2].x = 0;
		position[2].y = blockSize;
		position[3].x = blockSize;
		position[3].y = blockSize;
		break;
	case S_BLOCK:
		position[1].x = blockSize;
		position[1].y = 0;
		position[2].x = blockSize;
		position[2].y = -blockSize;
		position[3].x = 2 * blockSize;
		position[3].y = -blockSize;
		break;
	case T_BLOCK:
		position[1].x = blockSize;
		position[1].y = -blockSize;
		position[2].x = blockSize;
		position[2].y = 0;
		position[3].x = 2 * blockSize;
		position[3].y = 0;
		break;
	case Z_BLOCK:
		position[1].x = blockSize;
		position[1].y = 0;
		position[2].x = blockSize;
		position[2].y = blockSize;
		position[3].x = 2 * blockSize;
		position[3].y = blockSize;
		break;
	default:
		break;
	}
	// Rotate randomly the piece
	this->Rotate((rand() % 4) * 90);
}

Piece::Piece(const Piece& aPiece)
{
	color = aPiece.color;
	shape = aPiece.shape;
	blockSize = aPiece.blockSize;
	rotation = aPiece.rotation;
	for (int i = 0; i < 4; i++)
	{
		position[i].x = aPiece.position[i].x;
		position[i].y = aPiece.position[i].y;
	}
}

// DESTRUCTOR

Piece::~Piece() {}

// METHODS

void Piece::Display()
{
	Vector2 size{ blockSize,blockSize };	
	DrawRectangleV(position[0], size, color);
	DrawRectangleV(position[1], size, color);
	DrawRectangleV(position[2], size, color);
	DrawRectangleV(position[3], size, color);
}

void Piece::Move(Vector2 direction, int nbOfCases)
{
	for (int i = 0; i < 4; i++)
	{
		position[i] = Vector2Add(position[i], Vector2Scale(direction, blockSize*nbOfCases));
	}
}

void Piece::SetPosition(Vector2 newPosition)
{
	for (int i = 1; i < 4; i++)
	{
		position[i] = Vector2Add(newPosition, Vector2Subtract(position[i], position[0]));

	}
	position[0] = newPosition;
}

void Piece::Rotate(int angle)
{
	rotation = (rotation + angle) % 360;
	Vector2 centerOfRotation;
	switch (shape)
	{
	case I_BLOCK:
		centerOfRotation = position[2];
		break;
	case J_BLOCK:
		centerOfRotation = position[2];
		break;
	case L_BLOCK:
		centerOfRotation = position[2];
		break;
	case O_BLOCK:
		return;
		break;
	case S_BLOCK:
		centerOfRotation = position[1];
		break;
	case T_BLOCK:
		centerOfRotation = position[2];
		break;
	case Z_BLOCK:
		centerOfRotation = position[2];
		break;
	default:
		centerOfRotation = position[0];
		break;
	}

	for (int i = 0; i < 4; i++)
	{
		// Change of coordinates
		position[i].x -= centerOfRotation.x;
		position[i].y -= centerOfRotation.y;

		// Rotation
		position[i] = Vector2Rotate(position[i],angle);

		// Return to the originals coordinates
		position[i].x += centerOfRotation.x;
		position[i].y += centerOfRotation.y;

	}
}

// GETTER

Color Piece::GetColor()
{
	return color;
}

Shape Piece::GetShape()
{
	return shape;
}

int Piece::GetRotation()
{
	return rotation;
}

Vector2 Piece::GetPosition(int i)
{
	return position[i];
}

// OVERLOADING

Piece& Piece::operator= (const Piece& p)
{
	this->color = p.color;
	this->shape = p.shape;
	this->blockSize = p.blockSize;
	this->rotation = p.rotation;
	for (int i = 0; i < 4; i++)
	{
		this->position[i] = p.position[i];
	}
	return *this;
}