
#include "CameraPerspective.h"
#include "Screen.h"

void CameraPerspective::SetPerspective(float zFar, float zNear, float fov) {
	_zFar = zFar;
	_zNear = zNear;
	_fov = fov;

	_matProject = glm::perspective(_fov, Engine::Screen::aspect(), _zNear, _zFar);
	CameraTemp::MakeProjectView();
}

void CameraPerspective::Load(const Json::Value& data) {
	CameraTemp::Load(data);
}

void CameraPerspective::Save(Json::Value& data) {
	CameraTemp::Save(data);
}
