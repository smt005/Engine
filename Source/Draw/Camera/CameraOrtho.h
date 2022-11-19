#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json/json.h>

#include "Draw/Camera/CameraTemp.h"

class CameraOrtho : public CameraTemp {
public:
	typedef std::shared_ptr<CameraOrtho> Ptr;

public:
	inline void Resize() override {
		SetOrtho();
	}

	void SetOrtho(const float left = -1.f, const float right = 1.f, const float bottom = -1.f, const float top = 1.f);

public:
	void Load(const Json::Value& data) override;
	void Save(Json::Value& data) override;

private:
	float _left;
	float _right;
	float _bottom;
	float _top;
};
