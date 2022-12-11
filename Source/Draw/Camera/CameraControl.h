#pragma once

#include "Camera.h"

namespace Engine { class Callback; }

class CameraControl final : public Camera {
public:
	typedef std::shared_ptr<CameraControl> Ptr;

	enum class MoveDirect {
		NONE,
		FORVARD, BACK, LEFT, RIGHT, TOP, DOWN,
		FORVARD_HORIZONT, BACK_HORIZONT
	};

public:
	CameraControl() {}
	CameraControl(const Type type) { Camera::_type = type; }
	~CameraControl();

	void Load(const Json::Value& data) override;
	void Save(Json::Value& data) override;

	void Enable(const bool state);
	void MakeCallback();
	void Move(const MoveDirect direct, const float kForce = 1.f);
	template <typename T>
	void Move(const T& directVector, const float kForce = 1.f);
	void Rotate(const glm::vec2& angles);

	const float SetSpeed(const float speed) {
		_speed = speed;
		return _speed;
	}

private:
	float _speed = 1.f;
	float _angleSpeed = 0.005f;
	Engine::Callback* _callbackPtr = nullptr;
};
