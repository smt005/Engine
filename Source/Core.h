#pragma once

#include <string>
#include <memory>
#include <ctime>
#include "json/json.h"

#include "Game.h"

struct GLFWwindow;

namespace Engine
{

class Window;

class Core
{
public:
	static int execution(Game::Uptr& game);
	static bool main();
	static void close();
	static void mainLoop(GLFWwindow* window);

	static void init();
	static void update();
	static void draw();
	static void resize();
	static void log(const std::string& text);
	static const Json::Value& settingJson(const std::string& key);
	static double currentTime();
	static double deltaTime();

	static void SetCursorPos(const double x, const double y);
};

};	// Engine

#define LOG Engine::Core::log