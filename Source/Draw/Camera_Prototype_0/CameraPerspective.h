#pragma once

#include "CameraTemp.h"

class CameraPerspective : public CameraTemp {
public:
	typedef std::shared_ptr<CameraPerspective> Ptr;

public:
	inline void Resize() override {
		SetPerspective();
	}

	void Load(const Json::Value& data) override;
	void Save(Json::Value& data) override;

public:
	void SetPerspective(float zFar = 1000.f, float zNear = 0.1f, float fov = 45.f);

private:
	float _zFar;
	float _zNear;
	float _fov;
};
