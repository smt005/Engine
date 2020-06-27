#pragma once

#include <string>
#include "Common/DataClass.h"

using namespace std;

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Texture: public DataClass <Texture>
{
private:
	string _fileName;
	unsigned int _id = 0;

public:
	Texture() {}
	Texture(const string &name, bool needLoad = true);
	~Texture();

	const unsigned int& id();
	unsigned int load();
	unsigned int load(const string &newName);

public:
	static unsigned int loadTexture(const char* fileName);
};
