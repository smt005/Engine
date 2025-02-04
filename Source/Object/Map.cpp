
#include "Map.h"
#include "Object.h"

#include "Model.h"
#include "Physics/Physics.h"
#include "Draw/Camera/CameraControl.h"
#include "FileManager.h"
#include "Core.h"
#include "Common/Help.h"

#include <algorithm>

std::vector<Map::Ptr> Map::currentMaps;
Map Map::defaultMap;

Map::Ptr& Map::SetCurrentMap(Map::Ptr& map) {
	currentMaps.clear();
	return currentMaps.emplace_back(map);
}

Map::Ptr& Map::AddCurrentMap(Map::Ptr& map) {
	return currentMaps.emplace_back(map);
}

void Map::ClearCurrentMap(Map::Ptr& map) {
	currentMaps.clear();
}

Map::Map(const string& name)
	: _physicsState(false)
{
	create(name);
}

Map::~Map() {

}

bool Map::create(const string& name)
{
	setName(name);
	clear();
	return load();
}

void Map::clear() {
	objects.clear();
}

bool Map::load() {
	clear();

	string fileName = "Maps/" + getName() + ".json";
	Json::Value data;
	help::loadJson(fileName, data);

	// ���������
	{
		// ���
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

		// ������
		auto cameraData = data["camera"];
		if (cameraData) {
			const Json::Value& classData = cameraData["class"];
			const std::string classCamera = classData.isString() ? classData.asString() : std::string();

			const Json::Value& currentData = cameraData["current"];
			bool setCurrent = currentData.isBool() ? currentData.asBool() : false;

			if (classCamera == "CameraControl") {
				CameraControl* cameraTempPtr = new CameraControl();
				cameraTempPtr->Load(cameraData);
				cameraTempPtr->Enable(setCurrent);
				_cameraPtr = std::shared_ptr<Camera>(cameraTempPtr);
			} else {
				Camera* cameraTempPtr = new Camera();
				cameraTempPtr->Load(cameraData);
				_cameraPtr = std::shared_ptr<Camera>(cameraTempPtr);
			}

			_cameraPtr->Init();

			if (setCurrent) {
				Camera::Set<Camera>(_cameraPtr);
			}
		}
	}

	// �������
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

		Object& object = *objects.emplace_back(std::make_shared<Object>(name, modelName, pos));
		object.setVisible(visible);
		object.setTypeActorPhysics(typeActorPhysics);
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
	for (Object::Ptr obgectPtr : objects) { obgectPtr->releaseActorPhysics(); }
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
	for (auto object : additObjects) object->Action();
}

Object& Map::addObjectToPos(const string& nameModel, const glm::vec3& pos)
{
	Object& object = *objects.emplace_back(std::make_shared<Object>("", nameModel, pos));
	return object;
}

Object& Map::addObject(const string& nameModel, const glm::mat4x4& mat)
{
	// TODO: ��������
	glm::vec3 pos = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
	Object& object = *objects.emplace_back(std::make_shared<Object>("", nameModel, pos));
	return object;
}

Object& Map::addObject(Object* object)
{
	return *objects.emplace_back(object);
}

Object::Ptr& Map::addObject(Object::Ptr& object)
{
	return objects.emplace_back(object);
}

void Map::RemoveObject(const std::string& name)
{
	auto it = std::find_if(objects.begin(), objects.end(), [name](const Object::Ptr& object) { return object->getName() == name ? true : false; });
	if (it != objects.end()) {
		objects.erase(it);
	}
}

void Map::RemoveObject(const Object::Ptr& objectPtr)
{
	auto it = std::find_if(objects.begin(), objects.end(), [&objectPtr](const Object::Ptr& object) { return object == objectPtr ? true : false; });
	if (it != objects.end()) {
	objects.erase(it);
	}
}

void Map::RemoveObject(const Object* objectPtr)
{
	auto it = std::find_if(objects.begin(), objects.end(), [objectPtr](const Object::Ptr& object) { return object.get() == objectPtr ? true : false; });
	if (it != objects.end()) {
	objects.erase(it);
	}
}

void Map::AddToRemoveObject(const std::string& name)
{
	auto it = std::find_if(objects.begin(), objects.end(), [name](const Object::Ptr& object) { return object->getName() == name ? true : false; });
	if (it != objects.end()) {
		_toRemoveObject.emplace(it->get());
	}
}

void Map::AddToRemoveObject(const Object::Ptr& objectPtr)
{
	_toRemoveObject.emplace(objectPtr.get());
}

void Map::AddToRemoveObject(const Object* objectPtr)
{
	_toRemoveObject.emplace(const_cast<Object*>(objectPtr));
}

void Map::RemoveDeferredObjects()
{
	if (!_toRemoveObject.empty()) {
		objects.erase(std::remove_if(objects.begin(), objects.end(), [this](const Object::Ptr& objectPtr) {
			return _toRemoveObject.find(objectPtr.get()) != _toRemoveObject.end();
		}), objects.end());

		_toRemoveObject.clear();
	}
}

Object::Ptr	Map::getObjectPtrByName(const std::string& name) {
	auto it = std::find_if(objects.begin(), objects.end(), [name](const Object::Ptr& object) { return object->getName() == name ? true : false; });
	if (it == objects.end()) {
		return nullptr;
	}

	return *it;
}

Object& Map::getObjectByName(const std::string& name) {
	auto it = std::find_if(objects.begin(), objects.end(), [name](const Object::Ptr& object) { return object->getName() == name ? true : false; });
	if (it == objects.end()) {
		return _defaultObject;
	}

	return *(*it);
}

const Item::Ptr& Map::GetAdditObjectsByName(const std::string& name) const
{
	auto it = std::find_if(additObjects.begin(), additObjects.end(), [name](const auto& object) { return object->GetName() == name; });
	if (it == additObjects.end()) {
		return Item::defaultItemPtr;
	}
	return *it;
}
