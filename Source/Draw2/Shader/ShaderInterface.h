#pragma once
#include <string>
#include <memory>

class ShaderInterface {
public:
	using Ptr = std::shared_ptr<ShaderInterface>;

public:
	virtual ~ShaderInterface();

	virtual void Use();
	virtual void GetLocation();

	bool Init(const std::string& vertexFileName, const std::string& fragmentFileName);
	bool Load(const std::string& vertexFileName, const std::string& fragmentFileName);

protected:
	unsigned int _program = 0; // GLuint;
};
