#include "Position.h"

void Position::setMatrix(const float(&matrix)[16])
{
	_matrix[0][0] = matrix[0];
	_matrix[0][1] = matrix[1];
	_matrix[0][2] = matrix[2];
	_matrix[0][3] = matrix[3];

	_matrix[1][0] = matrix[4];
	_matrix[1][1] = matrix[5];
	_matrix[1][2] = matrix[6];
	_matrix[1][3] = matrix[7];

	_matrix[2][0] = matrix[8];
	_matrix[2][1] = matrix[9];
	_matrix[2][2] = matrix[10];
	_matrix[2][3] = matrix[11];

	_matrix[3][0] = matrix[12];
	_matrix[3][1] = matrix[13];
	_matrix[3][2] = matrix[14];
	_matrix[3][3] = matrix[15];
}
