
#pragma once

#include "Object/Triangle.h"
#include "Callback/Callback.h"

class Rect {
public:
	Rect();
	Rect(const unsigned int countPoligon);

	void init(const unsigned int countPoligon);

public:
	Triangle _triangle;
};

class Rect3 {
public:
	Rect3();
	~Rect3();

	void init();
	void getStrip(Triangle& triangle, const float texCoord_0, const float texCoord_1, const std::string& textureStr);
	bool hint();

public:
	Triangle* _triangles;

	int _testVar = 0;
};

class Rect3Callback : public Rect3, public Engine::Callback {};