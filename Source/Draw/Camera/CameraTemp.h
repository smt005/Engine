#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json/json.h>

#include "Screen.h"

class CameraTemp
{
public:
	typedef std::shared_ptr<CameraTemp> Ptr;

public:
	inline const glm::mat4x4& ProjectView() {
		return _matProjectView;
	}

	inline const float* ProjectViewFloat() {
		return value_ptr(_matProjectView);
	}

	inline void MakeProjectView() {
		_matProjectView = _matProject * _matView;
	}

	virtual void Resize() {}

	/*void SetPerspective(float zFar = 1000.f, float zNear = 0.1f, float fov = 45.f) {
		_zFar = zFar;
		_zNear = zNear;
		_fov = fov;

		_matProject = glm::perspective(_fov, Engine::Screen::aspect(), _zNear, _zFar);
		MakeProjectView();
	}

	void SetOrtho(const float left = -1.f, const float right = 1.f, const float bottom = -1.f, const float top = 1.f) {
		_left = left;
		_right = right;
		_bottom = bottom;
		_top = top;

		_matProject = glm::ortho(_left, _right, _bottom, _top);
		MakeProjectView();
	}

	void LookAt(const glm::vec3& eye, const glm::vec3& direct) {
		_eye = eye;
		_direct = direct;

		_matView = glm::lookAt(_eye, _eye + _direct, _up);
		MakeProjectView();
	}

	void SetPos(const glm::vec3& eye) {
		_eye = eye;

		_matView = glm::lookAt(_eye, _eye + _direct, _up);
		MakeProjectView();
	}

	void SetDirect(const glm::vec3& direct) {
		_direct = direct;

		_matView = glm::lookAt(_eye, _eye + _direct, _up);
		MakeProjectView();
	}*/

public:
	template<typename T>
	static std::shared_ptr<T> Set(std::shared_ptr<T> camera) {
		_currentCameraPtr = currentCameraPtr;
	}

	static CameraTemp::Ptr Get();
	static CameraTemp& GetLink();

	template<typename T>
	static T* GetPtr() {
		if (!_currentCameraPtr) {
			_currentCameraPtr = std::make_shared<T>();
			return static_cast<T*>(_currentCameraPtr.get());
		}
		CameraTemp* ptr = _currentCameraPtr.get();
		T* tPtr = dynamic_cast<T*>(ptr);
		return tPtr;
	}

	template<typename T>
	static std::shared_ptr<T> Make() {
		return std::make_shared<T>();
	}

	template<typename T>
	static std::shared_ptr<T> MakeAndSet() {
		std::shared_ptr<T> tempPtr(new T());
		_currentCameraPtr = tempPtr;
		return tempPtr;
	}

protected:
	glm::mat4x4 _matProject;
	glm::mat4x4 _matView;
	glm::mat4x4 _matProjectView;

protected:
	static CameraTemp::Ptr _currentCameraPtr;
};
