
#include "../Camera/CameraTemp.h"

CameraTemp::Ptr CameraTemp::_currentCameraPtr;

CameraTemp::Ptr CameraTemp::Get() {
	return _currentCameraPtr;
}

CameraTemp& CameraTemp::GetLink() {
	if (!_currentCameraPtr) {
		_currentCameraPtr = std::make_shared<CameraTemp>();
	}
	return *_currentCameraPtr;
}

void CameraTemp::Load(const Json::Value& data) {
	const Json::Value& posData = data["pos"];
	if (posData.isArray()) {
		_eye.x = posData[0].asFloat();
		_eye.y = posData[1].asFloat();
		_eye.z = posData[2].asFloat();
	}

	const Json::Value& directData = data["direct"];
	if (directData.isArray()) {
		_direct.x = directData[0].asFloat();
		_direct.y = directData[1].asFloat();
		_direct.z = directData[2].asFloat();
	}

	const Json::Value& upData = data["up"];
	if (upData.isArray()) {
		_up.x = upData[0].asFloat();
		_up.y = upData[1].asFloat();
		_up.z = upData[2].asFloat();
	}
}

void CameraTemp::Save(Json::Value& data) {
	if (_eye.x == 0.f && _eye.y == 0.f && _eye.z == 0.f) {
		data.removeMember("pos");
	}
	else {
		Json::Value& posData = data["pos"];
		posData.append(_eye.x);
		posData.append(_eye.y);
		posData.append(_eye.z);
	}

	if (_direct.x == 1.f && _direct.y == 0.f && _direct.z == 0.f) {
		data.removeMember("direct");
	}
	else {
		Json::Value& directData = data["direct"];
		directData.append(_direct.x);
		directData.append(_direct.y);
		directData.append(_direct.z);
	}

	if (_up.x == 0.f && _up.y == 0.f && _up.z == 1.f) {
		data.removeMember("up");
	}
	else {
		Json::Value& upData = data["up"];
		upData.append(_up.x);
		upData.append(_up.y);
		upData.append(_up.z);
	}
}
