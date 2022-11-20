#pragma once

#include "CameraProt2.h"

namespace Engine { class Callback; }

class CameraControl final : public CameraProt2 {
public:
	enum class MoveDirect {
		NONE,
		FORVARD, BACK, LEFT, RIGHT, TOP, DOWN,
		FORVARD_HORIZONT, BACK_HORIZONT
	};

public:
	~CameraControl();

	void Enable(const bool state);
	void MakeCallback();
	void Move(const MoveDirect direct, const float kForce = 1.f);
	template <typename T>
	void Move(const T& directVector, const float kForce = 1.f);
	void Rotate(const glm::vec2& angles);

private:
	float _speed = 1.f;
	float _angleSpeed = 0.005f;
	Engine::Callback* _callbackPtr = nullptr;
};
