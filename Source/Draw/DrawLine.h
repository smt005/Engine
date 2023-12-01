
#pragma once

#include <glm/glm.hpp>

class Line;
class Greed;

class DrawLine
{
public:
	static void prepare();
	static void draw(const Line& line, const glm::mat4x4& matrix);
	static void draw(const Line& line);
	static void draw(const Greed& greed);

	template <typename T>
	static void Draw(const T& line) {
		Draw(line.Data(), line.Count(), line.WidthLine(), line.Color(), line.Type());
		//Draw(0, 0, 0, nullptr, 0);
	}

	static void Draw(const float* data, unsigned int count, float widthLine, const float* color, unsigned short int type);
	static void SetIdentityMatrix();
	static void SetIdentityMatrixByPos(const float* pos);
};
