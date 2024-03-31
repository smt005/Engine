
#if !_DEBUG
	#include <Windows.h>
#endif

#include "Core.h"

int main(int argc, char** argv)
{
#if !_DEBUG
	FreeConsole();
#endif

	Engine::Game::Uptr gamePtr = Engine::Game::GetGame();
	return Engine::Core::execution(gamePtr);
}
