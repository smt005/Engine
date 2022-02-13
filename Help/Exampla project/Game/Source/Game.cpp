
#include "PhysX/MapPhysX.h"

#define NAME_GAME MapPhysX

Engine::Game::Ptr Engine::Game::GetGame(const std::string& params) {
	Engine::Game::Ptr gamePtr(new NAME_GAME());
	return gamePtr;
}
