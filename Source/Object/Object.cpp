
#include "Object.h"
#include "Model.h"
#include "Shape.h"
#include "Core.h"
#include "Screen.h"
#include "Draw/Camera.h"
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

void transformToScreen(glm::vec3& point, glm::mat4x4& mat)
{
	glm::vec4 p(point.x, point.y, point.z, 1.0f);
	p = mat * p;

	if (p.w != 1.0)
	{
		p.x /= p.w;
		p.y /= p.w;
		p.z /= p.w;
	}

	point.x = p.x;
	point.y = p.y;
	point.z = p.z;
}

void transformSizeToScreen(float& x, float& y, int& xRes, int& yRes)
{
	x = x * 0.5f + 0.5f;
	y = y * 0.5f + 0.5f;

	xRes = Engine::Screen::width() * x;
	yRes = Engine::Screen::height() * y;
}

bool hintTriangle(float* v0, float* v1, float* v2, int xTap, int yTap)
{
	//координаты вершин треугольника
	float x1 = v0[0], y1 = v0[1];
	float x2 = v1[0], y2 = v1[1];
	float x3 = v2[0], y3 = v2[1];

	//координаты произвольной точки
	float x = (float)xTap, y = (float)yTap;

	float a = (x1 - x) * (y2 - y1) - (x2 - x1) * (y1 - y);
	float b = (x2 - x) * (y3 - y2) - (x3 - x2) * (y2 - y);
	float c = (x3 - x) * (y1 - y3) - (x1 - x3) * (y3 - y);
	bool res = ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0)) ? true : false;
	return res;
}

bool hittingShape(Mesh& shape, glm::mat4x4& mat, glm::mat4x4& matCamera, int xTap, int yTap)
{
	for (int i = 0; i < shape.countIndex(); i += 3)
	{
		float v0[2];
		float v1[2];
		float v2[2];

		{
			int a = shape.indexes()[i];
			int index = 3 * a;
			//glm::vec3 point(shape.vertexes()[index], shape.vertexes()[index + 1], shape.vertexes()[index + 2]);
			glm::vec4 point4(shape.vertexes()[index], shape.vertexes()[index + 1], shape.vertexes()[index + 2], 1.0f);
			point4 = mat * point4;
			glm::vec3 point(point4.x, point4.y, point4.z);
			Engine::Core::log("hittingShape [0]: [" + std::to_string(point.x) + ", " + std::to_string(point.y) + ", " + std::to_string(point.z) + ']');

			transformToScreen(point, matCamera);
			Engine::Core::log("hittingShape [1]: [" + std::to_string(point.x) + ", " + std::to_string(point.y) + ", " + std::to_string(point.z) + ']');

			int xInt, yInt;
			transformSizeToScreen(point.x, point.y, xInt, yInt);
			Engine::Core::log("hittingShape [2]: [" + std::to_string(xInt) + ", " + std::to_string(yInt) + ']');

			v0[0] = xInt;
			v0[1] = yInt;
		}

		{
			int a = shape.indexes()[i + 1];
			int index = 3 * a;
			glm::vec4 point4(shape.vertexes()[index], shape.vertexes()[index + 1], shape.vertexes()[index + 2], 1.0f);
			point4 = mat * point4;
			glm::vec3 point(point4.x, point4.y, point4.z);
			Engine::Core::log("hittingShape [0]: [" + std::to_string(point.x) + ", " + std::to_string(point.y) + ", " + std::to_string(point.z) + ']');

			transformToScreen(point, matCamera);
			Engine::Core::log("hittingShape [1]: [" + std::to_string(point.x) + ", " + std::to_string(point.y) + ", " + std::to_string(point.z) + ']');

			int xInt, yInt;
			transformSizeToScreen(point.x, point.y, xInt, yInt);
			Engine::Core::log("hittingShape [2]: [" + std::to_string(xInt) + ", " + std::to_string(yInt) + ']');

			v1[0] = xInt;
			v1[1] = yInt;
		}

		{
			int a = shape.indexes()[i + 2];
			int index = 3 * a;
			glm::vec4 point4(shape.vertexes()[index], shape.vertexes()[index + 1], shape.vertexes()[index + 2], 1.0f);
			point4 = mat * point4;
			glm::vec3 point(point4.x, point4.y, point4.z);
			Engine::Core::log("hittingShape [0]: [" + std::to_string(point.x) + ", " + std::to_string(point.y) + ", " + std::to_string(point.z) + ']');

			transformToScreen(point, matCamera);
			Engine::Core::log("hittingShape [1]: [" + std::to_string(point.x) + ", " + std::to_string(point.y) + ", " + std::to_string(point.z) + ']');

			int xInt, yInt;
			transformSizeToScreen(point.x, point.y, xInt, yInt);
			Engine::Core::log("hittingShape [2]: [" + std::to_string(xInt) + ", " + std::to_string(yInt) + ']');

			v2[0] = xInt;
			v2[1] = yInt;
		}

		if (hintTriangle(v0, v1, v2, xTap, yTap))
		{
			Engine::Core::log("hittingShape [2]: [[" + std::to_string(v0[0]) + ", " + std::to_string(v0[1]) + "]], [["
											         + std::to_string(v1[0]) + ", " + std::to_string(v1[1]) + "]], [["
													 + std::to_string(v2[0]) + ", " + std::to_string(v2[1]) + "]] == ["
													 + std::to_string(xTap) + ", " + std::to_string(yTap) + "] = TRUE");
			return true;
		}
		else {
			Engine::Core::log("hittingShape [2]: [[" + std::to_string(v0[0]) + ", " + std::to_string(v0[1]) + "]], [["
				+ std::to_string(v1[0]) + ", " + std::to_string(v1[1]) + "]], [["
				+ std::to_string(v2[0]) + ", " + std::to_string(v2[1]) + "]] == ["
				+ std::to_string(xTap) + ", " + std::to_string(yTap) + "] = false");
		}
	}

	return false;
}

float Object::hit(const int x, const int y) {
	if (!_model) {
		return 0;
	}

	Engine::Core::log("hit: " + getName() + " [" + std::to_string(x) + ", " + std::to_string(y) + ']');

	glm::mat4x4 matCamera = Camera::getCurrent().matProjectView();
	if (hittingShape(_model->getMesh(), _matrix, matCamera, x, y)) {
		return 1;
	}
	else {
		return 0;
	}
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
