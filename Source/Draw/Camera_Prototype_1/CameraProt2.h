#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json/json.h>

#include "Screen.h"

class CameraProt2
{
public:
	typedef std::shared_ptr<CameraProt2> Ptr;

	enum class Type {
		PERSPECTIVE,
		ORTHO
	};

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

	inline void Resize(Type type) {
		_type = Type::PERSPECTIVE;
		if (_type == Type::PERSPECTIVE)
			SetPerspective();
		else
			SetOrtho();
	}

	inline void Resize() {
		if (_type == Type::PERSPECTIVE)
			SetPerspective();
		else
			SetOrtho();
	}

	inline const glm::vec3 Pos() {
		return _pos;
	}

	inline const glm::vec3 Direct() {
		return _direct;
	}

	inline const glm::vec3 Up() {
		return _up;
	}

	//...
	void SetPerspective(float zFar = 1000.f, float zNear = 0.1f, float fov = 45.f);
	void SetOrtho(const float left = -1.f, const float right = 1.f, const float bottom = -1.f, const float top = 1.f);

	void LookAt(const glm::vec3& pos, const glm::vec3& direct) {
		_pos = pos;
		_direct = direct;

		_matView = glm::lookAt(_pos, _pos + _direct, _up);
		MakeProjectView();
	}

	void SetPos(const glm::vec3& pos) {
		_pos = pos;

		_matView = glm::lookAt(_pos, _pos + _direct, _up);
		MakeProjectView();
	}

	void SetDirect(const glm::vec3& direct) {
		_direct = direct;

		_matView = glm::lookAt(_pos, _pos + _direct, _up);
		MakeProjectView();
	}

	virtual void Load(const Json::Value& data);
	virtual void Save(Json::Value& data);

public:
	template<typename T>
	static std::shared_ptr<T> Set(std::shared_ptr<T> camera) {
		_currentCameraPtr = currentCameraPtr;
	}

	static CameraProt2::Ptr Get() {
		return _currentCameraPtr;
	}

	template<typename T>
	static T* GetPtr() {
		if (!_currentCameraPtr) {
			_currentCameraPtr = std::make_shared<T>();
			return static_cast<T*>(_currentCameraPtr.get());
		}
		CameraProt2* ptr = _currentCameraPtr.get();
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

	static CameraProt2& GetLink();

private:
	glm::mat4x4 _matProject;
	glm::mat4x4 _matView;
	glm::mat4x4 _matProjectView;

	glm::vec3 _pos    { 0.f, 0.f, 0.f };
	glm::vec3 _direct { 1.f, 0.f, 0.f };
	glm::vec3 _up     { 0.f, 0.f, 1.f };

	Type _type = Type::PERSPECTIVE;

private:
	static CameraProt2::Ptr _currentCameraPtr;
};
