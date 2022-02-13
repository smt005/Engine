
#include "Game.h"

#define NAME_GAME Game

Engine::Game::Ptr Engine::Game::GetGame(const std::string& params) {
	Engine::Game::Ptr gamePtr(new NAME_GAME());
	return gamePtr;
}
