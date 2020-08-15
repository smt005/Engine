
#include "GL/glew.h"
#include "FreeImage.h"

#include <experimental\filesystem>

#include "Texture.h"
#include "FileManager.h"

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

Texture::Texture(const string &name, bool needLoad)
{
	setName(name);
	if (needLoad) load();
}

const unsigned int& Texture::id()
{
	if (_id != 0) return _id;
	_id = load();
	return _id;
}

unsigned int Texture::load()
{
	if (getName().empty()) return 0;

	_id = loadTexture(getName().c_str());

	return _id;
}

unsigned int Texture::load(const string &newName)
{
	if (newName.empty()) return 0;

	setName(newName);
	return load();
}

unsigned int Texture::loadTexture(const char* fileName)
{
	std::experimental::filesystem::path fullFileName = Engine::FileManager::fullPath(fileName);

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib(0);
	BYTE* bits(0);
	unsigned int width(0), height(0);
	GLuint _textureId;

	fif = FreeImage_GetFileType(fullFileName.string().c_str(), 0);

	if (fif == FIF_UNKNOWN)	{
		fif = FreeImage_GetFIFFromFilename(fullFileName.string().c_str());
	}

	if (fif == FIF_UNKNOWN) {
		return 0;
	}

	if (FreeImage_FIFSupportsReading(fif)) {
		dib = FreeImage_Load(fif, fullFileName.string().c_str());
	}

	if (!dib) {
		return 0;
	}

	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);

	if ((bits == 0) || (width == 0) || (height == 0)) {
		return 0;
	}

	GLenum format = GL_RGB;
	if (FreeImage_GetBPP(dib) == 32)format = GL_RGBA;
	if (FreeImage_GetBPP(dib) == 24)format = GL_RGB;
	if (FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;

	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bits);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return _textureId;
}
