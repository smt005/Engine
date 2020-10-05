
#include "Mesh.h"
#include "GL/glew.h"
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

	glGenBuffers(4, _buffer);

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
