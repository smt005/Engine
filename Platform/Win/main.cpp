
#include "Core.h"
#include "GamesManager.h"

int main(int argc, char** argv)
{
	Engine::GamePtr gamePtr;
	gamePtr = game::getGame(argc > 1 ? argv[1] : "MapExample");
	return Engine::Core::execution(gamePtr);
}
