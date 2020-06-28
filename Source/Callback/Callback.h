#pragma once

#include <map>
#include <vector>
#include <functional>
#include <memory>
#include "glm/vec2.hpp"

#include "CallbackEvent.h"
#include "VirtualKey.h"

namespace Engine
{

constexpr int CALLBACK_COUNT_KEY = 1024;
constexpr int CALLBACK_COUNT_TAP = 10;

class Callback;

enum class CallbackType
{
	NONE,
	PRESS_TAP,
	PINCH_TAP,
	RELEASE_TAP,
	PRESS_KEY,
	PINCH_KEY,
	RELEASE_KEY,
	MOVE
};

class Callback
{
public:
	static void addObject(Callback& object);
	static void removeObject(Callback& object);

	static void onPressKey(const int id);
	static void onReleaseKey(const int id);

	static void onPressTap(const int id);
	static void onReleaseTap(const int id);

	static void onMove(float x, float y);

	static void update();

	inline static const glm::vec2& mousePos() { return _mousePos; }
	inline static const glm::vec2& deltaMousePos() { return _deltaMousePos; }

	inline static bool pressKey(const VirtualKey id) { return _key[static_cast<int>(id)]; }
	inline static bool pressTap(const VirtualTap id) { return _tap[static_cast<int>(id)]; }

private:

	static void iteration(const CallbackType& type, const CallbackEventPtr& callbackEventPtr);

private:
	static std::map<int, Callback*> _objects;
	static glm::vec2 _mousePos;
	static glm::vec2 _deltaMousePos;
	static bool _key[CALLBACK_COUNT_KEY];
	static bool _tap[CALLBACK_COUNT_TAP];

public:
	Callback() {
		Callback::addObject(*this);
	}

	Callback(const CallbackType type, const std::function<void(const CallbackEventPtr& callbackEventPtr)>& function) {
		Callback::addObject(*this);
		add(type, function);
	}

	Callback(const CallbackType type, const std::function<void(const CallbackEventPtr& callbackEventPtr)>& function, int& id) {
		Callback::addObject(*this);
		add(type, function, id);
	}

	~Callback() {
		Callback::removeObject(*this);
	}

	int getId() {
		int id = reinterpret_cast<int>(this);
		return id;
	}

	int add(const CallbackType type, const std::function<void(const CallbackEventPtr& callbackEventPtr)>& function);
	int add(const CallbackType type, const std::function<void(const CallbackEventPtr& callbackEventPtr)>& function, int& id);
	void remove(const int id);
	void action(CallbackType type, const CallbackEventPtr& callbackEvent);

private:
	std::map<CallbackType, std::map<int, std::function<void(const CallbackEventPtr& callbackEventPtr)>>> _map;
};

};	// Engine