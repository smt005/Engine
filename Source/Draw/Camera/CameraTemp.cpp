
#include "../Camera/CameraTemp.h"

CameraTemp::Ptr CameraTemp::_currentCameraPtr;

CameraTemp::Ptr CameraTemp::CurrentPtr() {
	return _currentCameraPtr;
}

void CameraTemp::SetCurrent(CameraTemp::Ptr currentCameraPtr) {
	_currentCameraPtr = currentCameraPtr;
}
