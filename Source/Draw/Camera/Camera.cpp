
#include "Camera.h"
#include "PhysicPlane.h"
#include "Callback/Callback.h"

void Camera::Init() {
	_matView = glm::lookAt(_pos, _pos + _direct, _up);
	Resize();
}

void Camera::SetPerspective(const float zFar, const float zNear, const float fov) {
	_matProject = glm::perspective(fov, Engine::Screen::aspect(), zNear, zFar);
	MakeProjectView();
}

void Camera::SetOrtho(const float size, float zNear, float zFar) {
	float aspect = Engine::Screen::aspect();

	if (aspect > 1.f) {
		_matProject = glm::ortho(-size * aspect, size * aspect, -size, size, zNear, zFar);
	}
	else {
		_matProject = glm::ortho(-size, size, -size / aspect, size / aspect, zNear, zFar);
	}

	MakeProjectView();
}

glm::vec3 Camera::corsorCoord() {
	glm::vec2 mousePos = Engine::Callback::mousePos();
	mousePos.x += Engine::Screen::left();
	mousePos.y += Engine::Screen::top();

	glm::vec3 wincoord = glm::vec3(mousePos.x - Engine::Screen::left(), (Engine::Screen::height() - mousePos.y) + Engine::Screen::top(), 1.0f);
	glm::vec4 viewport = glm::vec4(0, 0, Engine::Screen::width(), Engine::Screen::height());

	glm::vec3 coord = glm::unProject(wincoord, _matView, _matProject, viewport);

	glm::vec3 vecCursor(_pos.x - coord.x, _pos.y - coord.y, _pos.z - coord.z);
	vecCursor = normalize(vecCursor);

	PhysicPlane plane;
	plane.set(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));

	glm::vec3 objcoord = plane.crossVector(vecCursor, _pos);

	return objcoord;
}

void Camera::Load(const Json::Value& data) {
	const Json::Value& typeData = data["type"];
	if (typeData.isString()) {
		const std::string typeStr = typeData.asString();

		if (typeStr == "ortho" || typeStr == "Ortho" || typeStr == "ORTHO") { // TODO:
			_type = Type::ORTHO;
		} else if (typeStr == "perspective" || typeStr == "Perspective" || typeStr == "PERSPECTIVE") {
			_type = Type::PERSPECTIVE;
		}
	}

	const Json::Value& posData = data["pos"];
	if (posData.isArray()) {
		_pos.x = posData[0].asFloat();
		_pos.y = posData[1].asFloat();
		_pos.z = posData[2].asFloat();
	}

	const Json::Value& directData = data["direct"];
	if (directData.isArray()) {
		_direct.x = directData[0].asFloat();
		_direct.y = directData[1].asFloat();
		_direct.z = directData[2].asFloat();
	}

	const Json::Value& upData = data["up"];
	if (upData.isArray()) {
		_up.x = upData[0].asFloat();
		_up.y = upData[1].asFloat();
		_up.z = upData[2].asFloat();
	}

	//...
	_matView = glm::lookAt(_pos, _pos + _direct, _up);
	Resize();
}

void Camera::Save(Json::Value& data) {
	data["type"] = _type == Type::PERSPECTIVE ? "perspective" : "ortho";

	if (_pos.x == 0.f && _pos.y == 0.f && _pos.z == 0.f) {
		data.removeMember("pos");
	}
	else {
		Json::Value& posData = data["pos"];
		posData.append(_pos.x);
		posData.append(_pos.y);
		posData.append(_pos.z);
	}

	if (_direct.x == 1.f && _direct.y == 0.f && _direct.z == 0.f) {
		data.removeMember("direct");
	}
	else {
		Json::Value& directData = data["direct"];
		directData.append(_direct.x);
		directData.append(_direct.y);
		directData.append(_direct.z);
	}

	if (_up.x == 0.f && _up.y == 0.f && _up.z == 1.f) {
		data.removeMember("up");
	}
	else {
		Json::Value& upData = data["up"];
		upData.append(_up.x);
		upData.append(_up.y);
		upData.append(_up.z);
	}
}

// STATIC
Camera::Ptr Camera::_currentCameraPtr;

Camera& Camera::GetLink() {
	if (!_currentCameraPtr) {
		_currentCameraPtr = std::make_shared<Camera>();
	}
	return *_currentCameraPtr;
}
