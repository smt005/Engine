
#include "Map.h"
#include "Object.h"
#include "Glider.h"
#include "Model.h"
#include "FileManager.h"
#include "Common/Help.h"

Map::Map(const string &name)
{
	create(name);
}

Map::~Map()
{
	help::clear(objects);
	help::clear(gliders);
}

bool Map::create(const string &name)
{
	setName(name);

	string fileName = "Maps/" + name + ".json";
	Json::Value data;
	help::loadJson(fileName, data);

	for (auto element : data["objects"])
	{
		const string &name = element["name"].asString();
		const string &modelName = element["model"].empty() ? "default" : element["model"].asString();
		const bool& visible = element["visible"].empty() ? true : element["visible"].asBool();

		vec3 pos(0.0f);
		int index = 0;
		auto elementPos = element["pos"];
		for (auto it = elementPos.begin(); it != elementPos.end(); ++it)
		{
			pos[index] = it->asFloat();
			++index;
		}

		Object& object = help::add(objects);
		object.set(name, modelName, pos);
		object.setVisible(visible);
	}

	for (auto element : data["gliders"])
	{
		const string &name = element["name"].asString();
		const string &modelName = element["model"].empty() ? "default" : element["model"].asString();
		const bool& visible = element["visible"].empty() ? true : element["visible"].asBool();

		vec3 pos(0.0f);
		int index = 0;
		auto elementPos = element["pos"];
		for (auto it = elementPos.begin(); it != elementPos.end(); ++it)
		{
			pos[index] = it->asFloat();
			++index;
		}

		Glider &object = help::add(gliders);
		object.set(name, modelName, pos);
		object.setVisible(visible);
	}

	return true;
}

void Map::getDataJson(Json::Value& dataJson)
{
	dataJson["name"] = getName();

	for (auto object : objects)
	{
		Json::Value dataObject;
		object->getDataJson(dataObject);
		dataJson["objects"].append(dataObject);
	}

	for (auto glider : gliders)
	{
		Json::Value dataObject;
		glider->getDataJson(dataObject);
		dataJson["gliders"].append(dataObject);
	}
}

void Map::action()
{
	for (auto object : objects) object->action();
	for (auto glider : gliders) glider->action();
}

Object& Map::addObjectToPos(const string& nameModel, const glm::vec3& pos)
{
	Object &object = help::add(objects);

	object.set("", nameModel, pos);

	return object;
}

Object& Map::addObject(const string& nameModel, const glm::mat4x4& mat)
{
	Object& object = help::add(objects);

	// TODO: Временно
	glm::vec3 pos = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
	object.set("", nameModel, pos);

	return object;
}

Object& Map::addObject(Object* object)
{
	return help::add(objects, object);
}
