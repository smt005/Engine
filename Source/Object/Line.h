#pragma once

#include <memory>
#include <vector>

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

#include "Color.h"
#include "Point.h"
#include "Position.h"

class Greed;

class Line
{
	friend Greed;

public:
	struct Point {
		float data[3] = { 0.0f, 0.0f, 0.0f };
	};

	enum
	{
		NONE = 0x0000,
		LINE = 0x0001,
		LOOP = 0x0002,
		STRIP = 0x0003
	};

public:
	Line()
		: _count(0)
		, _points(nullptr)
		, color(Color::RED)
		, _lineWidth(1.0f)
	{}
	Line(const float* const points, const unsigned int count, const unsigned short int type = LINE)
		: _count(0)
		, _points(nullptr)
		, color(Color::RED)
		, _lineWidth(1.0f)
	{
		set(points, count, type);
	}

	~Line();

	template <class PointT>
	void set(const PointT* const points, const unsigned int count, const unsigned short int type = LINE) {
		if (points == nullptr || count < 2) {
			return;
		}

		delete [] _points;
		_points = nullptr;
		_count = 0;

		_points = new Point[count];
		memcpy(_points, points, sizeof(Point) * count);
		_count = count;

		_type = type;
	}

	inline void setType(const unsigned short int type) { _type = type; }
	inline void setLineWidth(const float lineWidth) { _lineWidth = lineWidth; }

	inline const float* const getData() const { return (float*)_points; }
	inline unsigned int getCount() const { return _count; }
	inline unsigned short int getType() const { return _type; }
	inline float getLineWidth() const { return _lineWidth; }

	Line& operator=(const Line& line);

public:
	Color color;	// �������� TODO:

private:
	unsigned short int _type;	//	GL_LINES 0x0001, GL_LINE_LOOP 0x0002, GL_LINE_STRIP 0x0003
	float _lineWidth;
	unsigned int _count;
	Point* _points = nullptr;
};


class Greed : public Position
{
public:
	Greed()	{ }

	Greed(const float width, const float step = 1.0f, const std::vector<Color>& colors = std::vector<Color>()) {
		set(width, step, colors);
	}

	void set(const float width, const float step = 1.0f, const std::vector<Color>& colors = std::vector<Color>());

private:
	void generateLines(Line& line, const float width, const float step, const bool planeX);

public:
	Line lineX;
	Line lineY;
	Line lineZ;

	Line heavyLineX;
	Line heavyLineY;
	Line heavyLineZ;
};