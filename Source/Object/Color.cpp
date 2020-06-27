
#include "Color.h"

Color::Color()
{
	_data[RED] = 1.0f;
	_data[GREEN] = 1.0f;
	_data[2] = 1.0f;
	_data[ALPHA] = 1.0f;
}

Color::Color(const float r, const float g, const float b) {
	_data[RED] = r;
	_data[GREEN] = g;
	_data[2] = b;
	_data[ALPHA] = 1.0f;
}

Color::Color(const float r, const float g, const float b, const float a) {
	_data[RED] = r;
	_data[GREEN] = g;
	_data[BLUE] = b;
	_data[ALPHA] = a;
}

Color::Color(const int templateColor)
{
	switch (templateColor)
	{
	case RED: {
		_data[RED] = 1.0f;
		_data[GREEN] = 0.0f;
		_data[BLUE] = 0.0f;
		_data[ALPHA] = 1.0f;
	} break;
	case GREEN: {
		_data[RED] = 0.0f;
		_data[GREEN] = 1.0f;
		_data[BLUE] = 0.0f;
		_data[ALPHA] = 1.0f;
	} break;
	case BLUE: {
		_data[RED] = 0.0f;
		_data[GREEN] = 0.0f;
		_data[BLUE] = 1.0f;
		_data[ALPHA] = 1.0f;
	} break;
	case WHITE: {
		_data[RED] = 1.0f;
		_data[GREEN] = 1.0f;
		_data[BLUE] = 1.0f;
		_data[ALPHA] = 1.0f;
	} break;
	case BLACK: {
		_data[RED] = 0.0f;
		_data[GREEN] = 0.0f;
		_data[BLUE] = 0.0f;
		_data[ALPHA] = 1.0f;
	} break;
	default:
		break;
	}
}
