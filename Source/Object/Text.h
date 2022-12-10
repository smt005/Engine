#pragma once

#include <string>
#include <vector>
#include <stdint.h> // Поддержка uint8_t, int32_t и т.д.

namespace Engine {

class TextNew final {
public:
	TextNew() : _idTexture(0) {}
	TextNew(const std::string& text, const std::string& fontName = "Fonts/tahoma.ttf");

	inline const unsigned int IdTexture() { return _idTexture; }

	void SavePNG();

private:
	void MakeImageData();
	void MakeTexture();

public:
	//static void InitLib();
	static void RemoveLib();

private:
	unsigned int _idTexture;

	std::string _text;
	std::vector<uint8_t> _image;
	int32_t _width;
	int32_t _height;
};

}
