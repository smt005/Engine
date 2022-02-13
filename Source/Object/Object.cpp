
#include "Object.h"
#include "Model.h"
#include "Shape.h"
#include "Core.h"
#include "Physics/Physics.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Object::Object()
	: _createTime(Engine::Core::currentTime())
	,_typePhysics(Engine::Physics::Type::NONE)
	, _actorPhyscs(nullptr)
{

}

Object::Object(const string &name, const string &modelName, const vec3 &pos, const Json::Value &data)
	: _typePhysics(Engine::Physics::Type::NONE)
	, _actorPhyscs(nullptr)
{
	set(name, modelName, pos, data);
}

Object::~Object() {
	// Удалять актёров PhysX в деструкторе нельзя.
}

void Object::getDataJson(Json::Value& dataJson)
{
	dataJson["name"] = getName();
	dataJson["model"] = getModel().getName();

	dataJson["pos"][0] =_matrix[3][0];
	dataJson["pos"][1] = _matrix[3][1];
	dataJson["pos"][2] = _matrix[3][2];
}

Model& Object::getModel() {
	if (!_model) _model = Model::getByName("default");
	return *_model;
};

const float& Object::getHeight() {
	return _matrix[3][2];
}

void Object::set(const string &name, const string &modelName, const vec3 &pos, const Json::Value &data)
{
	setName(name);
	_model = Model::getByName(modelName);

	_matrix = translate(_matrix, pos);
	if (!data.empty()) setData(data);
}

void Object::setHeight(const float &height)
{
	_matrix[3][2] = height;
}

void Object::updateMatrixPhysics() {
	Engine::Physics::updateMatrixActor(*this);
}

bool Object::createActorPhysics() {
	return Engine::Physics::createActor(*this);
}

void Object::releaseActorPhysics() {
	Engine::Physics::releaseActor(*this);
}

void Object::addForce(const glm::vec3& vector, const Engine::Physics::Force& forceType) {
	Engine::Physics::addForceToActor(*this, vector, forceType);
}

// Virtual

void Object::setData(const Json::Value &data)
{
}

void Object::action()
{

}

Object* Object::_default = nullptr;

Object& Object::defaultItem()
{
	if (!_default)
	{
		_default = new Object();
		_default->setName("DEFAULT_OBJECT");
	}

	return *_default;
}

void Object::removeDefault()
{
	delete _default;
	_default = nullptr;
}
