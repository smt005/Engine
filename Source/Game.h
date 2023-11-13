#pragma once

#include <memory>
#include <string>
#include <filesystem>

namespace Engine
{

class Game
{
public:
	typedef std::unique_ptr<Game> Uptr;

public:
	virtual ~Game() = default;
	virtual std::string getName() { return typeid(*this).name(); }
	virtual std::filesystem::path getSourcesDir() { return std::string(); };
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	virtual void resize() {};
	virtual void close() {};

	static Game::Uptr GetGame(const std::string& params = std::string());
};

};	// Engine

/* Для подключения игры к движку, нужно в подмодуле сделать реалезацию метода GetGame.
Пример:
*/

/* Game.cpp
#include "Game.h"
#include "PhysX/MapPhysX.h"

Engine::Game::Ptr Engine::Game::GetGame(const std::string& params) {
	Engine::Game::Ptr gamePtr(new MapPhysX());
	return gamePtr;
}
*/