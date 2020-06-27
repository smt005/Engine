#pragma once

#include <string>

class Shader
{
public:
	static unsigned int getProgram(const std::string& vertexFileName, const std::string& fragmentFileName);

};
