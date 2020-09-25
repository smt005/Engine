#pragma once

#include <vector>
#include "json/json.h"

#include "Position.h"
#include "Texture.h"
#include "Color.h"

class Triangle : public Position, public Color
{
public:
	struct Point {
		float data[3] = { 0.0f, 0.0f, 0.0f };
	};
	
	struct TexCoord {
		float data[2] = { 0.0f, 0.0f };
	};

	enum
	{
		NONE = 0x0000,
		TRIANGLES = 0x0004,	// GL_TRIANGLES
		STRIP = 0x0005,		// GL_TRIANGLE_STRIP
		FAN = 0x0006		// GL_TRIANGLE_FAN
	};

public:
	Triangle()
		: _type(TRIANGLES)
		, _count(0)
		, _points(nullptr)
		, _texCoord(nullptr)
	{}
	~Triangle();

	inline void set(const TexturePtr& texture)				{ _texture = texture; }
	inline Triangle& operator=(const TexturePtr& texture)	{ _texture = texture; }
	inline void setTexture(const std::string& name)			{ _texture = Texture::getByName(name); }

	void setData(unsigned short int type, unsigned int count, Point* points, TexCoord* texCoord);

	unsigned int textureId() const {
		if (_texture) return _texture->id();
		TexturePtr& texture = getTextureStatic();
		return texture->id();
	}

	const Triangle& getSelf() const { return *this; }

	inline unsigned short int countVertex() const { return _count; }
	inline const float* const aVertex() const { return (float*)_points; }
	inline const float* const aTexCoord() const { return (float*)_texCoord; }
	inline const unsigned short int type() const { return _type; }

	inline bool hasVBO() const { return _hasVBO; }
	inline const unsigned int bufferVertexes() const { return _buffer[0]; }
	inline const unsigned int bufferTexCoords() const { return _buffer[1]; }

	bool const initVBO() const;

protected:
	unsigned short int _type;	//	GL_TRIANGLES 0x0004, GL_TRIANGLE_STRIP 0x0005, GL_TRIANGLE_FAN 0x0006
	unsigned int _count;
	Point* _points;
	TexCoord* _texCoord;
	TexturePtr _texture;

	bool _hasVBO = false;
	unsigned int _buffer[2];

private:
	static TexturePtr textureStatic;

public:
	static TexturePtr& getTextureStatic();
};
