
#pragma once

#include <Windows.h>
#include "GLFW/glfw3.h"
#include "json/json.h"

#include "Window.h"
#include "Core.h"
//#include "Draw/Draw.h"
//#include "Callback/Callback.h"

int windowWidth = 960;
int windowHeight = 540;
std::string windowTitle;

void cursorPositionCallback(GLFWwindow* Window, double x, double y);
void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);

using namespace Engine;

bool Window::create()
{
	GLFWwindow* window;

	if (!glfwInit())
		return false;

	const Json::Value& setting = Engine::Core::settingJson("window");
	if (!setting.empty()) {
		int windowWidthTemp = setting["width"].asInt();
		if (windowWidthTemp != 0) {
			windowWidth = windowWidthTemp;
		}

		int windowHeightTemp = setting["height"].asInt();
		if (windowHeightTemp != 0) {
			windowHeight = windowHeightTemp;
		}

		const std::string& text = setting["title"].asString();
		windowTitle = text.empty() ? "Game" : text;
	}

	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	glfwSetCursorPosCallback(window,	cursorPositionCallback);
	glfwSetMouseButtonCallback(window,	mouseButtonCallback);
	glfwSetKeyCallback(window,			keyCallback);

	glfwMakeContextCurrent(window);

	Core::init();

	while (!glfwWindowShouldClose(window))
	{
		//Callback::update();
		Core::update();
		Core::draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();

	return true;
}

int Window::width() {
	return windowWidth;
}

int Window::height() {
	return windowHeight;
}

float Window::aspect()
{
	if (windowHeight == 0 || windowWidth == 0) {
		return 1.0f;
	}

	return (static_cast<float>(windowWidth) / static_cast<float>(windowHeight));
}

//---

void cursorPositionCallback(GLFWwindow* Window, double x, double y)
{
	//Callback::onMove(static_cast<float>(x), static_cast<float>(y));
}

void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods)
{
	switch (Action)
	{
	case GLFW_PRESS: {
		//Callback::onPressTap(Button);
	}
	break;

	case GLFW_RELEASE: {
		//Callback::onReleaseTap(Button);
	}
	break;
	}
}

void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	switch (Action)
	{
	case GLFW_PRESS: {
		//Callback::onPressKey(Key);
	}
	break;

	case GLFW_RELEASE: {
		//Callback::onReleaseKey(Key);
	}
	break;
	}
}
