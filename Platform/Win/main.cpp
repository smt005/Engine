
#include <iostream>

#include "main.h"
#include "Engine.h"

#include "Core.h"
#include "GamesManager.h"

int main(int argc, char** argv)
{
	std::cout << "Hello, World!" << std::endl;
	std::cout << NAME_PROJECT << std::endl;
	
	Engine::exe();
	
	Engine::GamePtr gamePtr;
	gamePtr = game::getGame(argc > 1 ? argv[1] : "MapExample");
	return Engine::Core::execution(gamePtr);
}
