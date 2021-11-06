#pragma once

/* MyMatrix.h
* Author: Damien-Joseph RISPAL
* Date : 24/04/2021
* Description : Definition of a tempate class for matrix
*/

#include <iostream>
using namespace std;

template < typename T >
class MyMatrix
{
public:

	// Getter

	int GetNbRows()
	{
		return n;
	}

	int GetNbColumns()
	{
		return p;
	}

	T** GetMatrix()
	{
		return matrix;
	}

	T GetCase(int i, int j)
	{
		if (i < n && j < p && i >= 0 && j >= 0)
		{
			return matrix[i][j];
		}
		else
		{
			cerr << "ERROR: Matrix::GetCase needs correct parameters (" <<i<<":"<<j<<")" << endl;
			return matrix[0][0];
		}
	}

	// Setter

	void SetCase(int i, int j, T element)
	{
		if (i < n && j < p && i >= 0 && j >= 0)
		{
			matrix[i][j] = element;
		}
		else
		{
			cout << "ERROR: Matrix::SetCase needs correct parameters" << endl;
			// Just to verify in the console in debug mode if it is okay.
		}
	}

	void Fill(T element)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < p; j++)
			{
				matrix[i][j] = element;
			}
		}
	}

	// Constructor

	MyMatrix(int _nbOfRows, int _nbOfColumns, T element)
	{
		n = _nbOfRows;
		p = _nbOfColumns;
		matrix = new T * [n];
		if (matrix)
		{
			for (int i = 0; i < n; ++i)
			{
				matrix[i] = new T[p];
				for (int j = 0; j < p; j++)
				{
					matrix[i][j] = element;
				}
			}
		}
	}

	// Destructor 

	~MyMatrix()
	{
		if (matrix)
		{
			for (int i = 0; i < n; ++i)
			{
				delete[] matrix[i];
			}
		}
		delete[] matrix;
	}

private:
	int n; // Nb of rows
	int p; // Nb of columns
	T** matrix;

};
