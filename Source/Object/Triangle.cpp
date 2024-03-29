
#include "glad/gl.h"
#include <functional>

#include "Common/Help.h"
#include "FileManager.h"

#include "Triangle.h"

Texture::Ptr Triangle::textureStatic;

Triangle::~Triangle() {
	if (_points) {
		delete[] _points;
	}

	if (_texCoord) {
		delete[] _texCoord;
	}
}

void Triangle::setData(unsigned short int type, unsigned int count, Point* points, TexCoord* texCoord)
{
	type = _type;
	_count = count;
	_points = points;
	_texCoord = texCoord;
}

bool Triangle::initVBO() const
{
	unsigned int* buffer = const_cast<unsigned int*>(_buffer);

	glDeleteBuffers(2, buffer);
	glGenBuffers(2, buffer);

	if (_count == 0 || !_points) {
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, _count * 3 * sizeof(GLfloat), _points, GL_STATIC_DRAW);

	if (_texCoord) {
		glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
		glBufferData(GL_ARRAY_BUFFER, _count * 2 * sizeof(GLfloat), _texCoord, GL_STATIC_DRAW);
	}

	bool* hasVBO = const_cast<bool*>(&_hasVBO);
	*hasVBO = true;
	return _hasVBO;
}

// static

Texture::Ptr& Triangle::getTextureStatic() {
	if (!textureStatic) {
		textureStatic = Texture::getByName("Textures/Box.jpg");
	}

	return textureStatic;
}