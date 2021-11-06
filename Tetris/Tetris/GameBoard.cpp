/* GameBoard.cpp
* Author: Damien-Joseph RISPAL
* Date : 24/04/2021
* Description : Definition of the class GameBoard which handle the logic of the game.
*/

#include "GameBoard.h"
#include "MyMatrix.h"

#include <raylib.h>
#include <raymath.h>
#include <utility>
#include <iostream>
#include <cstdlib>
using namespace std;

// Constructor
 
GameBoard::GameBoard(int _width, int _height, int _size)
	:board(_height / _size, _width / _size, make_pair(Vector2{0,0},make_pair(false,BLACK)))
{
	width = _width;
	height = _height;
	caseSize = _size;
	shift.x = 200;
	shift.y = 40;

	cout << board.GetNbRows() << " " << board.GetNbColumns() << endl;

	for (int i = 0; i < board.GetNbRows(); i++)
	{
		for (int j = 0; j < board.GetNbColumns(); j++)
		{
			Vector2 position = {j*caseSize,i*caseSize};
			board.SetCase(i, j, make_pair(Vector2Add(position, shift), make_pair(false,BLACK)));
		}
	}
}

// Destructor

GameBoard::~GameBoard() {}


// Methods

void GameBoard::Display()
// Show the gameboard one the window
{
	Vector2 caseShape = { caseSize,caseSize };

	// outline of the board
	DrawRectangle(board.GetCase(0, 0).first.x - caseSize, board.GetCase(0, 0).first.y - caseSize, width + 2*caseSize, height + 2*caseSize, GRAY);

	for (int i = 0; i < board.GetNbRows(); i++)
	{
		for (int j = 0; j < board.GetNbColumns(); j++)
		{
			DrawRectangleV(board.GetCase(i,j).first,caseShape,board.GetCase(i, j).second.second);
		}
	}
}

void GameBoard::Clear()
// Delete all pieces on the board by turning the background black.
{
	for (int i = 0; i < board.GetNbRows(); i++)
	{
		for (int j = 0; j < board.GetNbColumns(); j++)
		{
			Vector2 position = board.GetCase(i, j).first;
			board.SetCase(i, j, make_pair(position, make_pair(false, BLACK)));
		}
	}
}

void GameBoard::ClearLine(int i)
// Clear the row i by turning its cases black.
{
	for (int j = 0; j < board.GetNbColumns(); j++)
	{
		Vector2 position = board.GetCase(i, j).first;
		board.SetCase(i, j, make_pair(position, make_pair(false, BLACK)));
	}
}

int GameBoard::LineFull()
// Check all lines that are fulfilled with pieces and delete them.
{
	int linefull = 0;
	for (int line = 1; line < board.GetNbRows(); line++)
	{
		int count = 0;
		while (count < board.GetNbColumns() && board.GetCase(line, count).second.first)
		{
			count++;
		}
		if (count == board.GetNbColumns())
		{
			linefull++;
			for (int row = line; row > 0; row--)
			{
				for (int column = 0; column < board.GetNbColumns(); column++)
				{
					this->SetCase(row, column, board.GetCase(row - 1, column).second.first, board.GetCase(row - 1, column).second.second);
				}
			}
		}
	}
	return linefull;
}


Vector2 GameBoard::coordToRowCol(Vector2 position)
// Transition from screen coordinates (in pixels) to logic coordinates.
{
	if (position.x >= shift.x && position.y >= shift.y && position.x < width + shift.x && position.y < height + shift.y)
	{
		Vector2 RowCol = { ((position.y - shift.y) / caseSize), ((position.x - shift.x) / caseSize) };
		//cout << "coordToRowCol(" << position.x << "," << position.y << ") = " << RowCol.x << ";" << RowCol.y << endl;
		return RowCol;
	}
	else
	{
		return Vector2{ -1,-1 };
	}
}

bool GameBoard::PieceCanDown(Piece aPiece)
// Chekc if a piece can move down on the gameboard.
{
	bool itCan = true;
	aPiece.Move(Vector2{ 0,1 }, 1);
	for (int i = 0; i < 4; i++)
	{
		Vector2 RowCol = coordToRowCol(aPiece.GetPosition(i));
		if (RowCol.x != -1 && RowCol.y != -1)
		{
			if (board.GetCase(RowCol.x, RowCol.y).second.first)
			{
				itCan = false;
			}
		}
		else
		{
			if (aPiece.GetPosition(i).y >= shift.y)
			{
				itCan = false;
			}
		}
	}
	return itCan;
}

bool GameBoard::PieceCanGo(Vector2 direction, Piece aPiece)
// Check if a piece can move to direction on the gameboard.
{
	bool itCan = true;
	aPiece.Move(direction,1);
	for (int i = 0; i < 4; i++)
	{
		Vector2 RowCol = coordToRowCol(aPiece.GetPosition(i));
		if (RowCol.x != -1 && RowCol.y != -1)
		{
			if (board.GetCase(RowCol.x, RowCol.y).second.first)
			{
				itCan = false;
			}
		}
		else
		{
			itCan = false;
		}
	}
	return itCan;
}

bool GameBoard::PieceCanRotate(Piece aPiece, int angle)
// Check if a piece have enough space to rotate.
{
	bool itCan = true;
	aPiece.Rotate(angle);
	for (int i = 0; i < 4; i++)
	{
		Vector2 RowCol = coordToRowCol(aPiece.GetPosition(i));
		if (RowCol.x != -1 && RowCol.y != -1)
		{
			if (board.GetCase(RowCol.x, RowCol.y).second.first)
			{
				itCan = false;
			}
		}
		else
		{
			itCan = false;
		}
	}
	return itCan;
}

// Arrays to get random piece and color
Shape shapes[7] = { I_BLOCK, J_BLOCK, L_BLOCK, O_BLOCK, S_BLOCK, T_BLOCK, Z_BLOCK };
Color colors[10] = { RED, GREEN, BLUE, YELLOW, DARKBLUE, BROWN, VIOLET, ORANGE, PINK, LIME };

Piece GameBoard::SpawnPiece()
// Create a randon piece with a random color on the top of the gameboard.
{
	// Add Color
	Piece piece(caseSize, shapes[rand()%7],colors[rand()%10]);
	// Move to the board
	piece.SetPosition(shift);
	// Move to a random case
	int i = (rand() % (board.GetNbColumns()-8)) +4;
	piece.Move(Vector2{ 1,0 }, i);
	return piece;
}

bool GameBoard::GameOver()
// Check if a piece is touching the top of the gameboard.
{
	int col = 0;
	while (col < board.GetNbColumns() && !board.GetCase(1, col).second.first) { col++; }
	return col != board.GetNbColumns();
}

// Setter

void GameBoard::SetCase(int i, int j, bool taken, Color color)
{
	Vector2 position = board.GetCase(i, j).first;
	board.SetCase(i, j, make_pair(position, make_pair(taken, color)));
}

void GameBoard::SetPiece(Piece aPiece)
{
	for (int i = 0; i < 4; i++)
	{
		Vector2 RowCol = coordToRowCol(aPiece.GetPosition(i));
		if (RowCol.x != -1 && RowCol.y != -1)
		{
			this->SetCase(RowCol.x, RowCol.y, true, aPiece.GetColor());
		}
	}

}

// Getter

int GameBoard::GetHeight()
{
	return height;
}

int GameBoard::GetWidth()
{
	return width;
}

Vector2 GameBoard::GetShift()
{
	return shift;
}

MyMatrix<pair<Vector2, pair <bool, Color>>> GameBoard::GetBoard()
{
	return board;
}

pair<Vector2, pair <bool, Color>> GameBoard::GetCase(int i, int j)
{
	return board.GetCase(i, j);
}
