
#include "CameraPerspective.h"

void CameraPerspective::Load(const Json::Value& data) {
	const Json::Value& posData = data["pos"];
	if (posData.isArray()) {
		_eye.x = posData[0].asFloat();
		_eye.y = posData[1].asFloat();
		_eye.z = posData[2].asFloat();
	}

	const Json::Value& directData = data["direct"];
	if (directData.isArray()) {
		_eye.x = directData[0].asFloat();
		_eye.y = directData[1].asFloat();
		_eye.z = directData[2].asFloat();
	}

	const Json::Value& upData = data["up"];
	if (upData.isArray()) {
		_up.x = upData[0].asFloat();
		_up.y = upData[1].asFloat();
		_up.z = upData[2].asFloat();
	}
}
