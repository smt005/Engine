
#pragma once

#include <glm/glm.hpp>

class Line;
class Greed;

namespace draw
{

class DrawLine
{
public:
	static void prepare();
	static void draw(const Line& line, const glm::mat4x4& matrix);
	static void draw(const Line& line);
	static void draw(const Greed& greed);
};

}