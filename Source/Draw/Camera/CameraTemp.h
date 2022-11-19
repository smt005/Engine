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

public:
	virtual void Resize() {}
	virtual void Load(const Json::Value& data) {}
	virtual void Save(Json::Value& data) {}

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
