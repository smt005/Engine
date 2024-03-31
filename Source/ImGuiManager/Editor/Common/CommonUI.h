// ◦ Xyz ◦

#pragma once

#include "imgui.h"
#include <memory>
#include <string>

namespace Editor {

	static ImVec4 redColor{ 0.8f, 0.35f, 0.35f, 1.0f };
	static ImVec4 greenColor{ 0.35f, 0.8f, 0.35f, 1.0f };
	static ImVec4 blueColor{ 0.35f, 0.35f, 0.8f, 1.0f };
	static ImVec4 whiteColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	static ImVec4 defaultColor{ 0.145f, 0.2939f, 0.453f, 1.0f };
	static ImVec4 disableColor{ 0.34f, 0.387f, 0.43f, 1.0f };
	static ImVec4 blackColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	static ImVec4 alphaColor{ 0.0f, 0.0f, 0.0f, 0.0f };

	typedef std::array<char, 128> TextChar;
	typedef std::shared_ptr<TextChar> TextCharPtr;
	typedef std::shared_ptr<std::string> stringPtr;

}
