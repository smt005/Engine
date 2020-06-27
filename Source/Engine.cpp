
#include "Engine.h"
#include "GameTest.h"
#include "Common/CommonEngine.h"

#include <iostream>
#include <string>

#include "json/json.h"

std::string stringFroJson(const Json::Value& value, const std::string& indentation = "\t")
{
	Json::StreamWriterBuilder writerBuilder;
	writerBuilder["indentation"] = indentation; // If you want whitespace-less output
	return Json::writeString(writerBuilder, value);
}

void Engine::exe() {
	std::cout << "Engine::exe()" << std::endl;
	GameTest::exe();
}

void Engine::exeForGame() {
	std::cout << "Engine::exeForGame()" << std::endl;
	CommonEngineFun();

	Json::Value settingJson;
	settingJson["window"]["width"] = 960;
	settingJson["window"]["height"] = 540;

	const std::string valueString = stringFroJson(settingJson);

	std::cout << "json: " << valueString << std::endl;
}
