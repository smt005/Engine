#pragma once

#include "Color.h"

class Point
{
public:
	Point() {
		_data[0] = 0.0f;
		_data[1] = 0.0f;
		_data[2] = 0.0f;
	}

	Point(const float* data) {
		memcpy(_data, data, sizeof(float) * 3);
	}

	Point(const float x, const float y, const float z) {
		_data[0] = x;
		_data[1] = y;
		_data[2] = z;
	}

	Point(const float* data, const Color& color = Color()) {
		memcpy(_data, data, sizeof(float) * 3);
		_color = color;
	}

	Point(const float x, const float y, const float z, const Color& color = Color()) {
		_data[0] = x;
		_data[1] = y;
		_data[2] = z;
		_color = color;
	}

	Color getColor() { return _color; }
	void setColor(const Color& color) { _color = color; }
	void setColor(const float* const color) { _color.set(color); }

private:
	float _data[3];
	Color _color;
};
