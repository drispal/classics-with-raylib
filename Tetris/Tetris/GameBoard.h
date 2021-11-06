#pragma once

/* GameBoard.h
* Author: Damien-Joseph RISPAL
* Date : 24/04/2021
* Description : Declaration of the class GameBoard which handle the logic of the game.
*/

#include <raylib.h>
#include <utility>
using namespace std;

#include "MyMatrix.h"
#include "piece.h"

class GameBoard
{
public:
    
    // Getter
    int GetHeight();
    int GetWidth();
    Vector2 GetShift();
    pair<Vector2, pair <bool, Color>> GetCase(int i, int j);
    MyMatrix<pair<Vector2, pair <bool, Color>>> GetBoard();

    // Setter
    void SetCase(int i, int j, bool taken, Color color);

    // Methods
    void Display(); 
    void Clear();
    void ClearLine(int i);
    int LineFull(); // Clear lines that are completed and return their count
    Vector2 coordToRowCol(Vector2 position);
    void SetPiece(Piece aPiece);
    bool PieceCanDown(Piece aPiece);
    bool PieceCanGo(Vector2 direction, Piece aPiece);
    bool PieceCanRotate(Piece aPiece, int angle);
    Piece SpawnPiece();
    bool GameOver(); // Return true when a piece hit the top of the board

    // Constructor 
    GameBoard(int _width, int _height, int _size);

    // Destructor
    virtual ~GameBoard();

protected:
    int width;
    int height;
    int caseSize; // Size of a case in screen pixel.
    Vector2 shift; // Sihft of the gameboard from the top left corner of the window.
    MyMatrix<pair<Vector2,pair <bool,Color>>> board;


};

