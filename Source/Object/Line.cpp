
#include "Line.h"

Line::~Line()
{
	if (_points) {
		delete[] _points;
		_points = nullptr;
	}
}

Line& Line::operator=(const Line& line) {
	if (line._points && line._count > 0) {
		_count = line._count;
		memcpy(_points, line._points, sizeof(Point) * _count);
		_type = line._type;
	}

	return *this;
}

//	Greed

void Greed::set(const float width, const float step,  const std::vector<Color>& colors)
{
	if (width == 0.0f || step == 0.0f) {
		return;
	}
	bool setUnicColor = colors.size() == 3;

	{
		generateLines(lineX, width, step, true);

		if (setUnicColor) {
			lineX.color = colors[0];
		} else {
			lineX.color = Color::GREEN;
			lineX.color.setAlpha(0.5f);
		}

		float heavyPoints[] = { 0.0f, 0.0f, 0.0f, 10.0f, 0.0f, 0.0f };
		heavyLineX.set(heavyPoints, 2);
		if (setUnicColor) {
			heavyLineX.color = colors[0];
		} else {
			heavyLineX.color = Color::GREEN;
			heavyLineX.color.setAlpha(0.25f);
		}

		heavyLineX.setLineWidth(5.0f);
	}

	{
		generateLines(lineY, width, step, false);

		if (setUnicColor) {
			lineY.color = colors[1];
		} else {
			lineY.color = Color::BLUE;
			lineY.color.setAlpha(0.5f);
		}

		float heavyPoints[] = { 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 0.0f };
		heavyLineY.set(heavyPoints, 2);

		if (setUnicColor) {
			heavyLineY.color = colors[1];
		} else {
			heavyLineY.color = Color::BLUE;
			heavyLineY.color.setAlpha(0.25f);
		}

		heavyLineY.setLineWidth(5.0f);
	}

	{
		float points[] = { 0.0f, 0.0f, -100.0f, 0.0f, 0.0f, 100.0f };
		lineZ.set(points, 2);

		if (setUnicColor) {
			lineZ.color = colors[2];
		} else {
			lineZ.color = Color::RED;
			lineZ.color.setAlpha(0.5f);
		}

		float heavyPoints[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f };
		heavyLineZ.set(heavyPoints, 2);

		if (setUnicColor) {
			heavyLineZ.color = colors[2];
		} else {
			heavyLineZ.color = Color::RED;
			heavyLineZ.color.setAlpha(0.25f);
		}

		heavyLineZ.setLineWidth(5.0f);
	}
}

void Greed::generateLines(Line& line, const float width, const float step, const bool planeX)
{
	int countLine = static_cast<int>(width / step);
	++countLine;
	int countPoint = countLine * 2;

	Line::Point* points = new Line::Point[countPoint];

	float startPos = width * 0.5f;
	float pos = -(width * 0.5f);

	for (int i = 0; i < countPoint; i += 2)
	{
		points[i].data[0] = planeX ? -startPos : pos;
		points[i].data[1] = planeX ?  pos : -startPos;
		points[i].data[2] = 0.0f;

		points[i + 1].data[0] = planeX ? startPos : pos;
		points[i + 1].data[1] = planeX ? pos : startPos;
		points[i + 1].data[2] = 0.0f;

		pos += step;
	}

	line.set(points, countPoint, Line::LINE);
}
