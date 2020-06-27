#pragma once

#include <memory>
#include <string>

namespace Engine
{
	
class Game
{
public:
	virtual ~Game() {};
	virtual std::string getName() = 0;
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
};

typedef std::shared_ptr<Game> GamePtr;


class DefaultGame final : public Game
{
public:
	std::string getName() override { return "DefaultGame"; }
};

};	// Engine
