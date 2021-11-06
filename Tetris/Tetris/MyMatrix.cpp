#include "MyMatrix.h"

template <typename T>
T MyMatrix <T>::GetCase(int i, int j)
{
	return matrix[i][j];
}

template <typename T>
void MyMatrix <T>::Fill(T number)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < p; j++)
		{
			matrix[i][j] = number;
		}
	}
}

template <typename T>
MyMatrix <T>::MyMatrix(int _nbOfRows, int _nbOfColumns)
{
	n = _nbOfRows;
	p = _nbOfColumns;
	//matrix = new T[n][p];
}

template <typename T>
MyMatrix <T>::~MyMatrix()
{
	delete[] matrix;
}