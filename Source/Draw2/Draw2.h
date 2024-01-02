#pragma once

#include <Object/Shape.h>

class Draw2 final {
public:
	static void SetClearColor(float r, float g, float b, float a);
	static void ClearColor();
	static void ClearDepth();
	static void Viewport();

	static void Draw(Shape& shape);

private:
	static unsigned int currentVAO;
	static unsigned int currentTexture;
};
