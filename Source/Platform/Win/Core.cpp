
#include "Core.h"
#include "Game.h"
#include "Screen.h"
#include "Callback/Callback.h"
#include "Common/Help.h"

#include "json/json.h"
#include "GLFW/glfw3.h"

#include <chrono>
#include <iostream>

using namespace Engine;

GamePtr _game = nullptr;
Json::Value _settingJson;
float _deltaTime = 0.0f;
double _lastTime = Core::currentTime();

void cursorPositionCallback(GLFWwindow* Window, double x, double y);
void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
void windowSizeCallback(GLFWwindow* window, int width, int height);

int Core::execution(const GamePtr& game)
{
	if (!game) return -1;

	if (!help::loadJson("Setting.json", _settingJson))
	{
		_settingJson.clear();
		_settingJson["window"]["width"] = 960;
		_settingJson["window"]["height"] = 540;
		_settingJson["window"]["title"] = "Window_default";
		_settingJson["window"]["fullscreen"] = false;
		help::saveJson("Setting.json", _settingJson);
	}

	_game = game;
	main();

	return 0;
}

bool Core::main() {
	GLFWwindow* window;

	if (!glfwInit())
		return false;

	Screen::init();

	window = glfwCreateWindow(Screen::width(), Screen::height(), Screen::title().c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	glfwMakeContextCurrent(window);

	Core::init();

	while (!glfwWindowShouldClose(window))
	{
		Callback::update();
		Core::update();
		Core::draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return true;
}

void Core::close()
{
	exit(1);
}

void Core::init()
{
	if (!_game) return;
	_game->init();
}

void Core::update()
{
	if (!_game) return;

	double currentTime = Core::Core::currentTime();
	double deltaTime = currentTime - _lastTime;
	_lastTime = currentTime;
	_deltaTime = static_cast<float>(deltaTime / 1000);

	_game->update();
}

void Core::draw()
{
	if (!_game) return;
	_game->draw();
}

void Core::resize()
{
	if (!_game) return;
	_game->resize();
}

void Core::log(const std::string& text)
{
#ifdef _DEBUG
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
#endif // DEBUG
}

const Json::Value& Core::settingJson(const std::string& key)
{
	if (key.empty()) {
		return _settingJson;
	}

	if (_settingJson[key]) {
		return _settingJson[key];
	}

	return _settingJson;
}

double Core::currentTime()
{
	std::chrono::milliseconds ms;
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	double value = static_cast<double>(ms.count());
	return value;
}

float Core::deltaTime() {
	return _deltaTime;
}

//...

void cursorPositionCallback(GLFWwindow* Window, double x, double y)
{
	Callback::onMove(static_cast<float>(x), static_cast<float>(y));
}

void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods)
{
	switch (Action)
	{
	case GLFW_PRESS: {
		Callback::onPressTap(Button);
	}
					 break;

	case GLFW_RELEASE: {
		Callback::onReleaseTap(Button);
	}
					   break;
	}
}

void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	switch (Action)
	{
	case GLFW_PRESS: {
		Callback::onPressKey(Key);
	}
					 break;

	case GLFW_RELEASE: {
		Callback::onReleaseKey(Key);
	}
					   break;
	}
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
	Screen::setWidth(width);
	Screen::setHeight(height);

	Core::resize();
}
