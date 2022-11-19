#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json/json.h>

#include "Screen.h"
#include "Draw/Camera/CameraTemp.h"

class CameraPerspective : public CameraTemp {
public:
	typedef std::shared_ptr<CameraPerspective> Ptr;

public:
	inline void Resize() override {
		SetPerspective();
	}

	void SetPerspective(float zFar = 1000.f, float zNear = 0.1f, float fov = 45.f) {
		_zFar = zFar;
		_zNear = zNear;
		_fov = fov;

		_matProject = glm::perspective(_fov, Engine::Screen::aspect(), _zNear, _zFar);
		CameraTemp::MakeProjectView();
	}

	void LookAt(const glm::vec3& eye, const glm::vec3& direct) {
		_eye = eye;
		_direct = direct;

		_matView = glm::lookAt(_eye, _eye + _direct, _up);
		CameraTemp::MakeProjectView();
	}

	void SetPos(const glm::vec3& eye) {
		_eye = eye;

		_matView = glm::lookAt(_eye, _eye + _direct, _up);
		CameraTemp::MakeProjectView();
	}

	void SetDirect(const glm::vec3& direct) {
		_direct = direct;

		_matView = glm::lookAt(_eye, _eye + _direct, _up);
		CameraTemp::MakeProjectView();
	}

private:
	float _zFar;
	float _zNear;
	float _fov;

};
