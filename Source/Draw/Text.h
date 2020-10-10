
#pragma once

#include <map>
#include <string>

namespace Engine {
	class Text {
	public:
		Text()
			: _textureId(-1)
			, _width(-1)
			, _height(-1)
		{}

		Text(const std::string& text, const std::string& fontName)
			: _textureId(-1)
			, _width(-1)
			, _height(-1)
		{
			set(text, fontName);
		}

		void set(const std::string& text, const std::string& fontName);

		/*void operator=(const std::string text) {
			set(text);
		}*/

	private:
		int _textureId;
		int _width;
		int _height;
		std::string _text;
		std::string _fontName;
	};

	class TextManager {
	public:
		static bool init();
		static void release();


	};
};	// Engine
