#pragma once

#include "Camera.h"

namespace Engine { class Callback; }

class CameraControlOutside final : public Camera {
public:
	typedef std::shared_ptr<CameraControlOutside> Ptr;

	enum class MoveDirect {
		NONE,
		FORVARD, BACK, LEFT, RIGHT, TOP, DOWN,
		FORVARD_HORIZONT, BACK_HORIZONT
	};

public:
	CameraControlOutside() {}
	CameraControlOutside(const Type type) { Camera::_type = type; }
	~CameraControlOutside();

	void Load(const Json::Value& data) override;
	void Save(Json::Value& data) override;

	void Enable(const bool state);
	void MakeCallback();
	void Move(const MoveDirect direct, const float kForce = 1.f);

	template <typename T>
	void Move(const T& directVector, const float kForce = 1.f);
	void Rotate(const glm::vec2& angles);

	const float& SetSpeed(const float speed) {
		_speed = speed;
		return _speed;
	}

	void SetPosOutside(const glm::vec3& posOutside, const float distOutside);
	void SetPosOutside(const glm::vec3& posOutside);
	void SetDistanceOutside(const float distOutside);

	const glm::vec3& GetPosOutside() const {
		return _posOutside;
	}
	float GetDistanceOutside() const {
		return _distOutside;
	}

	Engine::Callback* GetCallbackPtr() {
		return _callbackPtr;
	}

public:
	bool enableCallback = true;

private:
	glm::vec3 _posOutside = {0.f, 0.f, 0.f};
	float _distOutside = 100.f;

	float _speed = 1.f;
	float _angleSpeed = 0.005f;
	Engine::Callback* _callbackPtr = nullptr;
};
