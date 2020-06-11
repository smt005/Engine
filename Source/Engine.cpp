
#include <iostream>

#include "Engine.h"
#include "Game.h"


void Engine::exe() {
	std::cout << "Engine::exe()" << std::endl;
	Game::exe();
}

void Engine::exeForGame() {
	std::cout << "Engine::exeForGame()" << std::endl;
}
