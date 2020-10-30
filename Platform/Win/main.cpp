
#define WINDOWS_HIDE_CONSOLE 0

#if WINDOWS_HIDE_CONSOLE
	#include <Windows.h>
#endif

#include "Core.h"
#include "GamesManager.h"

int main(int argc, char** argv)
{
#if WINDOWS_HIDE_CONSOLE
	FreeConsole();
#endif

	Engine::GamePtr gamePtr;
	gamePtr = game::getGame(argc > 1 ? argv[1] : std::string());
	return Engine::Core::execution(gamePtr);
}
