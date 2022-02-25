
#include "Map.h"
#include "Object.h"
#include "Glider.h"
#include "Model.h"
#include "Physics/Physics.h"
#include "Draw/Camera.h"
#include "FileManager.h"
#include "Core.h"
#include "Common/Help.h"

#include <algorithm>

Map::Map(const string& name)
	: _physicsState(false)
{
	create(name);
}

Map::~Map() {
	help::clear(objects);
	help::clear(gliders);
}

bool Map::create(const string& name)
{
	setName(name);
	clear();
	return load();
}

void Map::clear() {
	help::clear(objects);
	help::clear(gliders);
}

bool Map::load() {
	clear();

	string fileName = "Maps/" + getName() + ".json";
	Json::Value data;
	help::loadJson(fileName, data);

	// Параметры
	{
		// Фон
		int index = 0;
		float color[4];
		auto elementColor = data["backColor"];
		for (auto it = elementColor.begin(); it != elementColor.end(); ++it) {
			color[index] = it->asFloat();
			++index;
			if (index > 4) {
				break;
			}
		}
		setColor(color);

		// Камера
		auto cameraData = data["camera"];
		if (cameraData) {
			_cameraPtr = std::make_shared<Camera>();
			_cameraPtr->setJsonData(cameraData);
			Camera::setCurrent(*_cameraPtr);
		}
	}

	// Объекты
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

		const string &physicsType = element["physics"].asString();
		Engine::Physics::Type typeActorPhysics = Engine::Physics::Type::NONE;
		if (physicsType == "convex") {
			typeActorPhysics = Engine::Physics::Type::CONVEX;
		} else if (physicsType == "triangle") {
			typeActorPhysics = Engine::Physics::Type::TRIANGLE;
		}

		Object& object = help::add(objects);
		object.set(name, modelName, pos);
		object.setVisible(visible);
		object.setTypeActorPhysics(typeActorPhysics);
	}

	// Глайдеры
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

void Map::initPhysixs() {
	if (!_physicsState) {
		for (auto object : objects) {
			object->createActorPhysics();
		}
		_physicsState = true;
	}
}

void Map::releasePhysixs() {
	for (Object* obgectPtr : objects) { obgectPtr->releaseActorPhysics(); }
	for (Glider* gliderPtr : gliders) { gliderPtr->releaseActorPhysics(); }

	_physicsState = false;
}

void Map::updatePhysixs() {
	if (!_physicsState) {
		return;
	}


	for (auto object : objects) {
		object->updateMatrixPhysics();
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

Object*	Map::getObjectPtrByName(const std::string& name) {
	auto it = std::find_if(objects.begin(), objects.end(), [name](const Object* object) { return object->getName() == name ? true : false; });
	if (it == objects.end()) {
		return nullptr;
		//help::log("Map::getObjectByName(" + name  + ") not found.");
	}

	return *it;
}

Object& Map::getObjectByName(const std::string& name) {
	auto it = std::find_if(objects.begin(), objects.end(), [name](const Object* object) { return object->getName() == name ? true : false; });
	if (it == objects.end()) {
		return _defaultObject;
	}

	return *(*it);
}

const Camera& Map::getCamera() {
	if (!_cameraPtr) {
		_cameraPtr = std::make_shared<Camera>();
	}
	return *_cameraPtr;
}
