
#include "CameraOrtho.h"
#include "Screen.h"

void CameraOrtho::Load(const Json::Value& data) {
}

void CameraOrtho::Save(Json::Value& data) {
}

void CameraOrtho::SetOrtho(const float left, const float right, const float bottom, const float top) {
	_left = left;
	_right = right;
	_bottom = bottom;
	_top = top;

	_matProject = glm::ortho(_left, _right, _bottom, _top);
	MakeProjectView();
}
