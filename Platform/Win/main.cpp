
#define WINDOWS_HIDE_CONSOLE 0

#if WINDOWS_HIDE_CONSOLE
	#include <Windows.h>
#endif

#include "Core.h"

int main(int argc, char** argv)
{
#if WINDOWS_HIDE_CONSOLE
	FreeConsole();
#endif

	Engine::Game::Ptr gamePtr = Engine::Game::GetGame();
	return Engine::Core::execution(gamePtr);
}
