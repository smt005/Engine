
#include "Draw/Text.h"
#include "Common/Help.h"
#include "FileManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Engine {
	std::map<std::string, FT_Face> _faces;
	bool isInited = false;

	static FT_Library  library;

	// Text
	
	void Text::set(const std::string& text, const std::string& font) {
		if (!isInited) {
			if (!TextManager::init()) {
				return;
			}
		}

		//...
	}

	// TextManager

	bool TextManager::init() {
		FT_Error err = FT_Init_FreeType(&library);
		if (err) {
			help::log("Freetype2 FT_Init_FreeType error: " + std::to_string(err));
			return false;
		}

		return isInited = true;
	}

	void TextManager::release() {

		// Удалить всё перед завершением

		FT_Done_FreeType(library);
		isInited = false;
	}

	//...

};
