
#include "Mesh.h"
#include "glad/gl.h"
#include <memory>

Mesh::~Mesh()
{
	_countVertex = 0;
	
	if (_aVertex)
	{
		delete[] _aVertex;
		_aVertex = nullptr;
	}

	if (_aNormal)
	{
		delete[] _aNormal;
		_aNormal = nullptr;
	}

	if (_aTexCoord)
	{
		delete[] _aTexCoord;
		_aTexCoord = nullptr;
	}

	_countIndex = 0;

	if (_aIndex)
	{
		delete[] _aIndex;
		_aIndex = nullptr;
	}
    
    if (_buffer[0]) {
		glDeleteBuffers(1, &_buffer[0]);
    }
    
    if (_buffer[1]) {
		 glDeleteBuffers(1, &_buffer[1]);
    }
    
    if (_buffer[2]) {
		glDeleteBuffers(1, &_buffer[2]);
    }
    
    if (_buffer[3]) {
		glDeleteBuffers(1, &_buffer[3]);
    }
}

bool Mesh::initVBO()
{
	glDeleteBuffers(4, _buffer);

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(4, _buffer);
	glBindVertexArray(_VAO);

	if (_countVertex == 0 || !_aVertex || _countIndex == 0 && !_aIndex) {
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 3 * sizeof(GLfloat), _aVertex, GL_STATIC_DRAW);

	if (_aTexCoord)	{
		glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
		glBufferData(GL_ARRAY_BUFFER, _countVertex * 2 * sizeof(GLfloat), _aTexCoord, GL_STATIC_DRAW);
	}
	else {
		_buffer[1] = 0;
	}

	if (_aNormal) {
		glBindBuffer(GL_ARRAY_BUFFER, _buffer[2]);
		glBufferData(GL_ARRAY_BUFFER, _countVertex * 3 * sizeof(GLfloat), _aNormal, GL_STATIC_DRAW);
	}
	else {
		_buffer[2] = 0;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _countIndex * sizeof(GLuint), _aIndex, GL_STATIC_DRAW);

	// ”станавливаем указатели на вершинные атрибуты 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_hasVBO = true;
	return _hasVBO;
}

void Mesh::copy(const Mesh& mesh)
{
	_countVertex = mesh._countVertex;

	_aVertex = new float[_countVertex * 3];
	_aNormal = new float[_countVertex * 3];

	size_t sizeVertex = sizeof(float) * _countVertex * 3;

	memcpy(_aVertex, mesh._aVertex, sizeVertex);
	memcpy(_aNormal, mesh._aNormal, sizeVertex);

	_aTexCoord = new float[_countVertex * 2];
	size_t sizeTexCoord = sizeof(float) * _countVertex * 2;
	memcpy(_aTexCoord, mesh._aTexCoord, sizeTexCoord);

	_countIndex = mesh._countIndex;

	_aIndex = new unsigned int[_countIndex * 3];
	size_t sizeIndex = sizeof(unsigned int) * _countIndex * 3;
	memcpy(_aIndex, mesh._aIndex, sizeIndex);
}

void Mesh::setData(	float* const aVertex,
					float* const aNormal,
					float* const aTexCoord,
					const unsigned int countVertex,
					unsigned int* const aIndex,
					const unsigned int countIndex)
{
	_aVertex = aVertex;
	_aNormal = aNormal;
	_aTexCoord = aTexCoord;
	_countVertex = countVertex;

	_aIndex = aIndex;
	_countIndex = countIndex;
}

void Mesh::MakeRectangle(Mesh& mesh) {
	mesh._countVertex = 4;

	//...
	mesh._aVertex = new float[mesh._countVertex * 3];

	mesh._aVertex[0] = 1;
	mesh._aVertex[1] = 0;
	mesh._aVertex[2] = -1;

	mesh._aVertex[3] = -1;
	mesh._aVertex[4] = 0;
	mesh._aVertex[5] = -1;

	mesh._aVertex[6] = 1;
	mesh._aVertex[7] = 0;
	mesh._aVertex[8] = 1;

	mesh._aVertex[9] = -1;
	mesh._aVertex[10] = 0;
	mesh._aVertex[11] = 1;

	//...
	mesh._aNormal = new float[mesh._countVertex * 3];

	mesh._aNormal[0] = 0;
	mesh._aNormal[1] = 1;
	mesh._aNormal[2] = 0;

	mesh._aNormal[3] = 0;
	mesh._aNormal[4] = 1;
	mesh._aNormal[5] = 0;

	mesh._aNormal[6] = 0;
	mesh._aNormal[7] = 1;
	mesh._aNormal[8] = 0;

	mesh._aNormal[9] = 0;
	mesh._aNormal[10] = 1;
	mesh._aNormal[11] = 0;

	//...
	mesh._aTexCoord = new float[mesh._countVertex * 2];

	mesh._aTexCoord[0] = 1;
	mesh._aTexCoord[1] = 1;

	mesh._aTexCoord[2] = 0;
	mesh._aTexCoord[3] = 1;

	mesh._aTexCoord[4] = 1;
	mesh._aTexCoord[5] = 0;

	mesh._aTexCoord[6] = 0;
	mesh._aTexCoord[7] = 0;

	//...
	mesh._countIndex = 6;
	mesh._aIndex = new unsigned int[mesh._countIndex];

	mesh._aIndex[0] = 1;
	mesh._aIndex[1] = 2;
	mesh._aIndex[2] = 0;
	mesh._aIndex[3] = 1;
	mesh._aIndex[4] = 3;
	mesh._aIndex[5] = 2;
}
