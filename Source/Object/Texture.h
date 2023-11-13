#pragma once

#include <string>
#include "Common/DataClass.h"

class Texture: public DataClass <Texture>
{
public:
	typedef std::shared_ptr<Texture> Ptr;

private:
	std::string _fileName;
	unsigned int _id = 0;
	unsigned int _width = 0;
	unsigned int _height = 0;

public:
	Texture() {}
	Texture(const std::string &name, bool needLoad = true);
	~Texture();

	unsigned int id();
	unsigned int load();
	unsigned int load(const std::string &newName);

	void SetId(const unsigned int id);

public:
	static unsigned int loadTexture(const char* fileName);
	static unsigned int LoadTexture(const std::string& fileName, unsigned int& width, unsigned int& height);
};
