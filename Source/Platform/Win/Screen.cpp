
#pragma once

#include <Windows.h>
#include "json/json.h"

#include "Screen.h"
#include "Core.h"
#include "Draw/Draw.h"
#include "Callback/Callback.h"
#include "Callback/Event.h"

using namespace Engine;

float Screen::_width = 960;
float Screen::_height = 540;
std::string Screen::_title = "Window";

void Screen::init() {
	const Json::Value& setting = Engine::Core::settingJson("window");
	if (!setting.empty()) {
		int windowWidthTemp = setting["width"].asInt();
		if (windowWidthTemp != 0) {
			Screen::_width = windowWidthTemp;
		}

		int windowHeightTemp = setting["height"].asInt();
		if (windowHeightTemp != 0) {
			Screen::_height = windowHeightTemp;
		}

		const std::string& text = setting["title"].asString();
		Screen::_title = text.empty() ? "Game" : text;
	}
}

void Screen::resize() {
	//Resize::actionEvent();
}
