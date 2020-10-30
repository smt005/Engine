#pragma once

//#include "Callback/Event.h"

namespace Engine
{

class Screen
{
public:
	static void init();
	static inline float width() { return _width; }
	static inline float height() { return _height; }
	static inline float left() { return _left; }
	static inline float top() { return _top; }
	static inline std::string title() { return _title; }
	static inline float aspect() { return _height != 0 ? _width / _height : 1.0f; }

public:
	static inline void setWidth(const float width) { _width = width; }
	static inline void setHeight(const float height) { _height = height; }
	static inline void setLeft(const float left) { _left = left; }
	static inline void setTop(const float top) { _top = top; }
	//static void resize();

private:
	static float _width;
	static float _height;
	static float _left;
	static float _top;
	static std::string _title;
};
};	// Engine
