#pragma once

#include <string>
#include <vector>
#include <stdint.h> // Поддержка uint8_t, int32_t и т.д.

#include "Object/Color.h"

namespace Engine {

class Text final : public Color {
public:
	Text() : _idTexture(0), _size(1) {}
	Text(const std::string& text, const unsigned int size, const std::string& fontName = "Fonts/tahoma.ttf");

	inline const unsigned int IdTexture() { return _idTexture; }

	void Draw();
	void SavePNG();

	inline int32_t Width() { return _width; }
	inline int32_t Height() { return _height; }
	inline 	float Size() { return _size; }

private:
	void MakeImageData(const std::string& fontName);
	void MakeTexture();

public:
	//static void InitLib();
	static void RemoveLib();

private:
	unsigned int _idTexture;

	std::string _text;
	unsigned int _size;

	std::vector<uint8_t> _image;
	int32_t _width;
	int32_t _height;

public:
	float _offsetX = 0.f;
	float _offsetY = 0.f;
	float _offsetZ = 0.f;
};

}
