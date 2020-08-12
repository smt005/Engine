
#include "UISample.h"
#include "GL/glew.h"
#include "Common\Help.h"

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

		_points[0].data[0] = -0.5f;	_points[0].data[1] = 0.5f;	_points[0].data[2] = height;
		_points[1].data[0] = -0.5f;	_points[1].data[1] = -0.5f;	_points[1].data[2] = height;
		_points[2].data[0] = 0.5f;	_points[2].data[1] = 0.5f;	_points[2].data[2] = height;
		_points[3].data[0] = 0.5f;	_points[3].data[1] = -0.5f;	_points[3].data[2] = height;

		/*_points[0].data[0] = -1.0f;	_points[0].data[1] = 1.0f;	_points[0].data[2] = height;
		_points[1].data[0] = -1.0f;	_points[1].data[1] = -1.0f;	_points[1].data[2] = height;
		_points[2].data[0] = 1.0f;	_points[2].data[1] = 1.0f;	_points[2].data[2] = height;
		_points[3].data[0] = 1.0f;	_points[3].data[1] = -1.0f;	_points[3].data[2] = height;*/

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

	bool Sample::hit(const float mouseX, const float mouseY) {
		if (!_points &&  _count) {
			return false;
		}

		float left = 9999.9f;
		float right = -9999.9f;
		float top = -9999.9f;
		float bottom = 9999.9f;

		for (size_t i = 0; i < _count; ++i) {
			Point &point = _points[i];

			/*if (point.data[0] < left) left = point.data[0];
			if (point.data[0] > right) right = point.data[0];
			if (point.data[1] < bottom) bottom = point.data[1];
			if (point.data[1] > top) top = point.data[1];*/

			glm::vec4 vec4Temp(point.data[0], point.data[1], point.data[2], 0.0f);
			glm::vec4 vec4 = getMatrix() * vec4Temp;

			help::log("vec: [" + std::to_string(vec4Temp.x) + ", " + std::to_string(vec4Temp.y) + "] [" + std::to_string(vec4.x) + ", " + std::to_string(vec4.y) + "] ");

			if (vec4.x < left) left = vec4.x;
			if (vec4.x > right) right = vec4.x;
			if (vec4.y < bottom) bottom = vec4.y;
			if (vec4.y > top) top = vec4.y;
		}

		if (left < mouseX && mouseX < right && top > mouseY && mouseY > bottom) {
			help::log("Rect     : [" + std::to_string(left) + ", " + std::to_string(right) + "] [" + std::to_string(top) + ", " + std::to_string(bottom) + "] YES");
			return true;
		}

		help::log("Rect     : [" + std::to_string(left) + ", " + std::to_string(right) + "] [" + std::to_string(top) + ", " + std::to_string(bottom) + "] no");
		return false;
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
