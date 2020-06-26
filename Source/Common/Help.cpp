
#include <glm/glm.hpp>
#include "Help.h"
#include "FileManager.h"

#include <iostream>

using namespace glm;

bool help::loadJson(const std::string& fileName, Json::Value& value)
{
	std::string mystring = Engine::FileManager::readTextFile(fileName);

	Json::CharReaderBuilder readerBuilder;
	Json::CharReader *reader = readerBuilder.newCharReader();
	std::string err;
	if (reader->parse(mystring.c_str(), mystring.c_str() + mystring.length(), &value, &err)) {
		return true;
	}

	return false;
}

bool help::saveJson(const std::string& fileName, const Json::Value& value, const std::string& indentation)
{
	const std::string valueString = stringFroJson(value, indentation);
	return Engine::FileManager::writeTextFile(fileName, valueString);
}

std::string help::stringFroJson(const Json::Value& value, const std::string& indentation)
{
	Json::StreamWriterBuilder writerBuilder;
	writerBuilder["indentation"] = indentation; // If you want whitespace-less output
	return Json::writeString(writerBuilder, value);
}

bool help::intersection(vec3 start1, vec3 end1, vec3 start2, vec3 end2, vec3& out_intersection)
{
	vec3 dir1 = end1 - start1;
	vec3 dir2 = end2 - start2;

	//считаем уравнения прямых проходящих через отрезки
	float a1 = -dir1.y;
	float b1 = +dir1.x;
	float d1 = -(a1*start1.x + b1 * start1.y);

	float a2 = -dir2.y;
	float b2 = +dir2.x;
	float d2 = -(a2*start2.x + b2 * start2.y);

	//подставляем концы отрезков, для выяснения в каких полуплоскотях они
	float seg1_line2_start = a2 * start1.x + b2 * start1.y + d2;
	float seg1_line2_end = a2 * end1.x + b2 * end1.y + d2;

	float seg2_line1_start = a1 * start2.x + b1 * start2.y + d1;
	float seg2_line1_end = a1 * end2.x + b1 * end2.y + d1;

	//если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
	if (seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0)
		return false;

	float u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
	out_intersection = start1 + u * dir1;

	return true;
}
