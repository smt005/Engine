
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
