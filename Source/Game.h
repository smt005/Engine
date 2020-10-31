#pragma once

#include <memory>
#include <string>
#include <filesystem>

namespace Engine
{
	
class Game
{
public:
	virtual ~Game() {};
	virtual std::string getName() = 0;
	virtual std::filesystem::path getreSourcesDir() = 0;
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	virtual void resize() {};
	virtual void close() {};
};

typedef std::shared_ptr<Game> GamePtr;


class DefaultGame final : public Game
{
public:
	std::string getName() override { return "DefaultGame"; }
	std::filesystem::path getreSourcesDir() override { return ""; }
};

};	// Engine
