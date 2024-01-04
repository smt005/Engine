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

private:
	unsigned int _program = 0; // GLuint;

public:
	static Shader2::Ptr current;

};
