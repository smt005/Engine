
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

Object::Object(const string &name, const string &modelName, const vec3 &pos)
	: _typePhysics(Engine::Physics::Type::NONE)
	, _actorPhyscs(nullptr)
{
	set(name, modelName, pos);
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

float Object::hit(const int xTap, const int yTap) {
	auto transformToScreen = [](glm::vec3& point, glm::mat4x4& mat) {
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
	};

	auto transformSizeToScreen = [](float& x, float& y, int& xRes, int& yRes) {
		x = x * 0.5f + 0.5f;
		y = y * 0.5f + 0.5f;

		xRes = Engine::Screen::width() * x;
		yRes = Engine::Screen::height() * y;
	};

	auto hintTriangle = [](float* v0, float* v1, float* v2, int xTap, int yTap) {
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
	};

	if (!_model) {
		return 0;
	}

	glm::mat4x4 matCamera = Camera::getCurrent().matProjectView();
	const Mesh& mesh = _model->getMesh();

	for (int index = 0; index < mesh.countIndex(); index += 3)
	{
		float vertexScreen[3][2];

		for (int iShift = 0; iShift < 3; ++iShift) {
			int a = mesh.indexes()[index + iShift];
			int index = 3 * a;
			glm::vec4 point4(mesh.vertexes()[index], mesh.vertexes()[index + 1], mesh.vertexes()[index + 2], 1.0f);
			point4 = _matrix * point4;

			glm::vec3 point(point4.x, point4.y, point4.z);
			transformToScreen(point, matCamera);

			int xInt, yInt;
			transformSizeToScreen(point.x, point.y, xInt, yInt);

			vertexScreen[iShift][0] = xInt;
			vertexScreen[iShift][1] = yInt;
		}

		if (hintTriangle(vertexScreen[0], vertexScreen[1], vertexScreen[2], xTap, yTap)) {
			return true;
		}
	}

	return false;
}

void Object::set(const string &name, const string &modelName, const vec3 &pos)
{
	setName(name);
	_model = Model::getByName(modelName);

	_matrix = translate(_matrix, pos);
}

void Object::setHeight(const float &height)
{
	_matrix[3][2] = height;
}

void Object::setActorPos(const glm::vec3& pos) {
	Engine::Physics::setPositionToActor(*this, pos);
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

Object::Ptr Object::hitObjects(const int x, const int y, const std::vector<Object::Ptr>& objects) {
	std::map<std::string, Object::Ptr> objectsUnderMouse;

	for (Object::Ptr object : objects) {
		if (object->visible() && object->hit(x, y)) {
			objectsUnderMouse.emplace(object->getName(), object);
		}
	}

	if (hitObjects(x, y, objects, objectsUnderMouse)) {
		return objectsUnderMouse.begin()->second;
	}

	return Object::Ptr(new Object());
}

bool Object::hitObjects(int x, int y, const std::vector<Object::Ptr>& objects, std::map<std::string, Object::Ptr>& objectsUnderMouse) {
	for (Object::Ptr object : objects) {
		if (object->visible() && object->hit(x, y)) {
			objectsUnderMouse.emplace(object->getName(), object);
		}
	}
	return !objectsUnderMouse.empty();
}
