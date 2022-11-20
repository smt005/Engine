#pragma once

#include "CameraTemp.h"

class CameraOrtho : public CameraTemp {
public:
	typedef std::shared_ptr<CameraOrtho> Ptr;

public:
	inline void Resize() override {
		SetOrtho();
	}

	void Load(const Json::Value& data) override;
	void Save(Json::Value& data) override;

public:
	void SetOrtho(const float left = -1.f, const float right = 1.f, const float bottom = -1.f, const float top = 1.f);

private:
	float _left;
	float _right;
	float _bottom;
	float _top;
};
