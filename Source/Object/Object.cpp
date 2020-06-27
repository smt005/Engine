
#include "Object.h"
#include "Model.h"
#include "Shape.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Object::Object()
{

}

Object::Object(const string &name, const string &modelName, const vec3 &pos, const Json::Value &data)
{
	set(name, modelName, pos, data);
}

Object::~Object()
{
}

void Object::getDataJson(Json::Value& dataJson)
{
	dataJson["name"] = getName();
	dataJson["model"] = getModel().getName();

	dataJson["pos"][0] =_matrix[3][0];
	dataJson["pos"][1] = _matrix[3][1];
	dataJson["pos"][2] = _matrix[3][2];
}

Model& Object::getModel()
{
	if (!_model) _model = Model::getByName("default");
	return *_model;
};

const float& Object::getHeight()
{
	return _matrix[3][2];
}

void Object::set(const string &name, const string &modelName, const vec3 &pos, const Json::Value &data)
{
	setName(name);
	_model = Model::getByName(modelName);

	if (length(pos) > 0.0f) _matrix = translate(_matrix, pos);
	if (!data.empty()) setData(data);
}

void Object::setHeight(const float &height)
{
	_matrix[3][2] = height;
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
