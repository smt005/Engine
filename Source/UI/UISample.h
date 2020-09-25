
#pragma once

#include <memory>
#include <vector>
#include "Object/Position.h"
#include "../Source/Object/Texture.h" 

namespace UI {
	class Sample;
	typedef std::shared_ptr<Sample> SamplePtr;

	class Sample : public Position
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
		Sample();
		~Sample();
		inline bool hasVBO() const { return _hasVBO; }
		const bool initVBO() const;

	public:
		TexturePtr _texture;
	
	public:
		unsigned short int _type;
		unsigned int _count;
		Point* _points;
		TexCoord* _texCoord;



		std::vector<SamplePtr> _childs;

		bool _hasVBO = false;
		unsigned int _buffer[2];
	};
}
