#pragma once

#include <memory>

class Color
{
public:
	enum
	{
		RED,
		GREEN,
		BLUE,
		ALPHA,
		WHITE,
		BLACK
	};

public:
	Color();
	Color(const float* const data) { memcpy(_data, data, (sizeof(float) * 4)); }
	Color(const float r, const float g, const float b);
	Color(const float r, const float g, const float b, const float a);
	Color(const int templateColor);

	//inline Color& operator=(const int templateColor) { *this = Color(templateColor); }
	inline Color& operator=(const Color& color)		{ memcpy(_data, color._data, (sizeof(float) * 4));	return *this; }
	inline Color& operator=(const float* const data){ memcpy(_data, data, (sizeof(float) * 4));			return *this; }
	inline bool operator==(const Color& color) {
		if (_data[RED] == color._data[RED] && _data[GREEN] == color._data[GREEN] && _data[BLUE] == color._data[BLUE] && _data[ALPHA] == color._data[ALPHA])  return true;
		return false;
	}

	inline float& operator[](const size_t index) { return _data[index]; }
	
	inline void setColor(const Color& color)		{ memcpy(_data, color._data, (sizeof(float) * 4)); }
	inline void set(const float* const data){ memcpy(_data, data, (sizeof(float) * 4)); }

	inline void setRed(float const r)	{ _data[RED] = r;	}
	inline void setGreen(float const g)	{ _data[GREEN] = g;	}
	inline void setBlue(float const b)	{ _data[BLUE] = b;	}
	inline void setAlpha(float const a) { _data[ALPHA] = a;	}

	inline float getRed()	const { return _data[RED];	}
	inline float getGreen()	const { return _data[GREEN];	}
	inline float getBlue()	const { return _data[BLUE];	}
	inline float getAlpha()	const { return _data[ALPHA];	}

	inline const float* const getDataPtr() const { return _data; }

private:
	float _data[4];

private:
	static float _dataStatic[4];
};
