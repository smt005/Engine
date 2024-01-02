#pragma once
#include <string>

class Shader2 {
public:
	Shader2() = default;
	Shader2(const std::string& vertexFileName, const std::string& fragmentFileName);
	virtual ~Shader2();
	virtual void Bind();

	bool Load(const std::string& vertexFileName, const std::string& fragmentFileName);
	
private:
	unsigned int _program = 0; // GLuint;
};
