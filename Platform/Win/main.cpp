
#include <iostream>

#include "main.h"
#include "Engine.h"

int main(int argc, char** argv)
{
	std::cout << "Hello, World!" << std::endl;
	std::cout << NAME_PROJECT << std::endl;
	
	Engine::exe();
	
	return 0;
}
