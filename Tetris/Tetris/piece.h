/* piece.h
* Author: Damien-Joseph RISPAL
* Date : 24/04/2021
* Description : Declaration of the class Piece which implement all kind of Tetris pieces.
*/

#pragma once

#include <raylib.h>

// Different shapes of a Tetris pieces
enum Shape { I_BLOCK, J_BLOCK, L_BLOCK, O_BLOCK, S_BLOCK, T_BLOCK, Z_BLOCK };

class Piece
{
   
public:
 
    // GETTER

    Color GetColor(); // Give the color of the piece
    Shape GetShape(); // Give the shape of the piece (based on enum Shape)
    int GetRotation(); // Give the rotation of the piece
    Vector2 GetPosition(int i); // Give the array of positions of each blocks of a piece 

    // METHODS

    void Display(); // Draw the piece
    void Move(Vector2 direction, int nbOfCases);
    void SetPosition(Vector2 newPosition);
    void Rotate(int angle); // Clockwise

    // OVERLOADING
    Piece& operator = (const Piece& p);

    Piece(const Piece& aPiece); // Copy constructor
    Piece(int _blockSize, Shape _shape, Color _color); // Constructor

    virtual ~Piece(); // Destructor

    
protected: 
    Color color; // Color of the piece
    Shape shape; // Shape of the piece based on enum Shape
    int blockSize; // Size of the side of each blocks of a piece
    int rotation; // Rotation of the piece
    Vector2 position[4]; // Positions of each blocks of a piece


};


