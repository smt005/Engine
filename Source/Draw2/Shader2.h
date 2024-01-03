#pragma once
#include <string>
#include <MyStl/shared.h>

class Shader2 {
public:
	using Ptr = mystd::shared<Shader2>;

public:
	Shader2() = default;
	Shader2(const std::string& vertexFileName, const std::string& fragmentFileName);
	virtual ~Shader2();
	
	virtual void Use();
	virtual void GetLocation();

	bool Load(const std::string& vertexFileName, const std::string& fragmentFileName);
	
public:
	unsigned int u_matProjectionView;
	unsigned int u_matViewModel;

	unsigned int a_position;
	unsigned int a_texCoord;
	unsigned int a_normal;

	unsigned int s_baseMap;
	unsigned int u_color;

private:
	unsigned int _program = 0; // GLuint;

public:
	static Shader2::Ptr current;

};
