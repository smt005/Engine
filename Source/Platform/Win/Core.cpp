
#include "Core.h"
#include <windows.h>
#include "Platform/CompileParams.h"
#include "Game.h"
#include "Screen.h"
#include "Callback/Callback.h"
#include "FileManager.h"
#include "Common/Help.h"
#include "ImGuiManager/UI.h"
#include "json/json.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h" // https://www.glfw.org/docs/3.3/window_guide.html

#if THREAD_EXPAMPLE
#include <thread>
#include <atomic>
#endif

using namespace Engine;

#if _DEBUG
	#define ApplicationInfo	" v.0.0 DEBUG [" __DATE__"  " __TIME__" ]"
#else
	#define ApplicationInfo ""
#endif


Game::Uptr _game;
Json::Value _settingJson;
double _deltaTime = 0.0f;
double _lastTime = Engine::Core::currentTime();
const std::string fileNameSetting = "Setting.json";

void cursorPositionCallback(GLFWwindow* Window, double x, double y);
void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void windowPosCallback(GLFWwindow* window, int left, int top);
void windowCloseCallback(GLFWwindow* window);
void windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

typedef std::function<void(GLFWwindow* window)> WhileFunction;
std::vector<WhileFunction> whileFunctions;

int Engine::Core::execution(Game::Uptr& game)
{
	if (!game) {
		return -1;
	}

	std::swap(_game, game);

	const std::filesystem::path sourcesDir = _game->getSourcesDir();
	Engine::FileManager::setResourcesDir(sourcesDir);

	if (!help::loadJson(fileNameSetting, _settingJson)) {
		_settingJson.clear();
		_settingJson["window"]["width"] = 960;
		_settingJson["window"]["height"] = 540;
		_settingJson["window"]["left"] = 200;
		_settingJson["window"]["top"] = 100;
		_settingJson["window"]["title"] = "Window_default";
		_settingJson["window"]["fullscreen"] = false;
		_settingJson["window"]["resizable"] = true;

		help::saveJson(fileNameSetting, _settingJson);
	}

	main();

	return 0;
}

bool Engine::Core::main() {
	GLFWwindow* window;

	if (!glfwInit()) {
		return false;
	}

	// Set all the required options for GLFW
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int resizable = 1;
	if (_settingJson["window"]["resizable"].isBool()) {
		resizable = _settingJson["window"]["resizable"].asBool() ? GL_TRUE : GL_FALSE;
	}
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	Screen::init();

	std::string title = Screen::title() + ApplicationInfo;
	window = glfwCreateWindow(Screen::width(), Screen::height(), title.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		return false;
	}

	glfwSetWindowPos(window, Screen::left(), Screen::top());
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	glfwSetWindowPosCallback(window, windowPosCallback);
	glfwSetWindowCloseCallback(window, windowCloseCallback);
	glfwSetScrollCallback(window, windowScrollCallback);

	glfwMakeContextCurrent(window);

	int version = gladLoadGL(glfwGetProcAddress);
	printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Core::init();
	Core::resize();

#if EDITORS_ENABLED
	UI::Init(window);
#endif

	mainLoop(window);

#if EDITORS_ENABLED
	UI::Cleanup();
#endif

	glfwTerminate();

	return true;
}

void Engine::Core::close()
{
	if (_game) {
		_game->close();
	}

	// Сохранение настроек окна
	_settingJson["window"]["width"] = Screen::width();
	_settingJson["window"]["height"] = Screen::height();
	_settingJson["window"]["left"] = Screen::left();
	_settingJson["window"]["top"] = Screen::top();

	help::saveJson(fileNameSetting, _settingJson);

	exit(1);
}

void Engine::Core::init()
{
	if (!_game) return;
	_game->init();
}

void Engine::Core::update()
{
	if (!_game) return;

	double currentTime = Core::Core::currentTime();
	double deltaTime = currentTime - _lastTime;
	_lastTime = currentTime;
	_deltaTime = deltaTime / 1000;

	_game->update();
}

void Engine::Core::draw()
{
	if (!_game) return;
	_game->draw();
}

void Engine::Core::resize()
{
	if (!_game) return;
	_game->resize();
}

void Engine::Core::log(const std::string& text)
{
#ifdef _DEBUG
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
#endif // DEBUG
}

const Json::Value& Engine::Core::settingJson(const std::string& key)
{
	if (key.empty()) {
		return _settingJson;
	}

	if (_settingJson[key]) {
		return _settingJson[key];
	}

	return _settingJson;
}

double Engine::Core::currentTime()
{
	std::chrono::milliseconds ms;
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	double value = static_cast<double>(ms.count());
	return value;
}

double Engine::Core::deltaTime() {
	return _deltaTime;
}

void Engine::Core::SetCursorPos(const double x, const double y) {

	whileFunctions.emplace_back([x, y](GLFWwindow* window) {
		Engine::Callback::setMousePos(x, y);
		glfwSetCursorPos(window, x, y);
	});
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
	} break;

	case GLFW_RELEASE: {
		Callback::onReleaseKey(Key);
	} break;
	}
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
	Screen::setWidth(width);
	Screen::setHeight(height);

	Engine::Core::resize();
}

void windowPosCallback(GLFWwindow* window, int left, int top)
{
	Screen::setLeft(left);
	Screen::setTop(top);

	Engine::Core::resize();
}

void windowCloseCallback(GLFWwindow* window)
{
	if (_game) {
		_game->close();
	}

	// Сохранение настроек окна
	_settingJson["window"]["width"] = Screen::width();
	_settingJson["window"]["height"] = Screen::height();
	_settingJson["window"]["left"] = Screen::left();
	_settingJson["window"]["top"] = Screen::top();

	help::saveJson(fileNameSetting, _settingJson);
}

void windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Callback::onScroll(yoffset);
}

#if THREAD_EXPAMPLE
void Core::mainLoop(GLFWwindow* window) {
	std::atomic_bool alive = true;

	std::thread threadUpdate([&alive]() {
	while (alive.load(std::memory_order_relaxed)) {
		Core::update();
	}});

	while (!glfwWindowShouldClose(window)) {
		Callback::update();
		Core::draw();

		for (WhileFunction function : whileFunctions) {
			function(window);
		}
		whileFunctions.clear();

#if EDITORS_ENABLED
		UI::Update();
		UI::Render();
#endif

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	alive.store(false, std::memory_order_relaxed);
	threadUpdate.join();
}
#else
void Engine::Core::mainLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(window))) {
		Callback::update();
		Core::update();
		Core::draw();

		for (WhileFunction function : whileFunctions) {
			function(window);
		}
		whileFunctions.clear();

#if EDITORS_ENABLED
		UI::Update();
		UI::Render();
#endif

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
#endif
