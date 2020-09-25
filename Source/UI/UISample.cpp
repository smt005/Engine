
#include "UISample.h"

#include "GL/glew.h"

namespace UI {
	Sample::Sample()
		: _type(STRIP)
	{
		_count = 4;
		const float height = 0.0f;

		_points = new Point[_count];

		/*_points[0].data[0] = 0.0f;	_points[0].data[1] = 1.0f;	_points[0].data[2] = height;
		_points[1].data[0] = 0.0f;	_points[1].data[1] = 0.0f;	_points[1].data[2] = height;
		_points[2].data[0] = 1.0f;	_points[2].data[1] = 1.0f;	_points[2].data[2] = height;
		_points[3].data[0] = 1.0f;	_points[3].data[1] = 0.0f;	_points[3].data[2] = height;*/

		/*_points[0].data[0] = -0.5f;	_points[0].data[1] = 0.5f;	_points[0].data[2] = height;
		_points[1].data[0] = -0.5f;	_points[1].data[1] = -0.5f;	_points[1].data[2] = height;
		_points[2].data[0] = 0.5f;	_points[2].data[1] = 0.5f;	_points[2].data[2] = height;
		_points[3].data[0] = 0.5f;	_points[3].data[1] = -0.5f;	_points[3].data[2] = height;*/

		_points[0].data[0] = -1.0f;	_points[0].data[1] = 1.0f;	_points[0].data[2] = height;
		_points[1].data[0] = -1.0f;	_points[1].data[1] = -1.0f;	_points[1].data[2] = height;
		_points[2].data[0] = 1.0f;	_points[2].data[1] = 1.0f;	_points[2].data[2] = height;
		_points[3].data[0] = 1.0f;	_points[3].data[1] = -1.0f;	_points[3].data[2] = height;

		_texCoord = new TexCoord[_count];
		_texCoord[0].data[0] = 0.0f;	_texCoord[0].data[1] = 1.0f;
		_texCoord[1].data[0] = 0.0f;	_texCoord[1].data[1] = 0.0f;
		_texCoord[2].data[0] = 1.0f;	_texCoord[2].data[1] = 1.0f;
		_texCoord[3].data[0] = 1.0f;	_texCoord[3].data[1] = 0.0f;

		_texture = Texture::getByName("Test/Plane.png");
	}

	Sample::~Sample() {
		if (_points) {
			delete[] _points;
		}

		if (_texCoord) {
			delete[] _texCoord;
		}
	}

	const bool Sample::initVBO() const
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

}
