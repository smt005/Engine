
#include "Camera.h"
#include "Screen.h"
#include "Callback/Callback.h"
#include "Object/PhysicPlane.h"

//#include <corecrt_math_defines.h>
#define M_PI 3.14159265359

Camera::Camera()
{
	setDefault();
}

Camera::~Camera() { }

// PRIVATE

inline void Camera::makeMatProjectView()
{
	_matProjectView = _matProject * _matView;

	if (_calcFrustum)
	{
		makeFrustum();
	}
}

void Camera::makeFrustum()
{
	float *clip = value_ptr(_matProjectView);

	// Находим A, B, C, D для ПРАВОЙ плоскости
	_frustum[0].x = clip[3] - clip[0];
	_frustum[0].y = clip[7] - clip[4];
	_frustum[0].z = clip[11] - clip[8];
	_frustum[0].w = clip[15] - clip[12];
	normalize(_frustum[0]);

	// Находим A, B, C, D для ЛЕВОЙ плоскости
	_frustum[1].x = clip[3] + clip[0];
	_frustum[1].y = clip[7] + clip[4];
	_frustum[1].z = clip[11] + clip[8];
	_frustum[1].w = clip[15] + clip[12];
	normalize(_frustum[1]);

	// Находим A, B, C, D для НИЖНЕЙ плоскости
	_frustum[2].x = clip[3] + clip[1];
	_frustum[2].y = clip[7] + clip[5];
	_frustum[2].z = clip[11] + clip[9];
	_frustum[2].w = clip[15] + clip[13];
	normalize(_frustum[2]);

	// ВЕРХНЯЯ плоскость
	_frustum[3].x = clip[3] - clip[1];
	_frustum[3].y = clip[7] - clip[5];
	_frustum[3].z = clip[11] - clip[9];
	_frustum[3].w = clip[15] - clip[13];
	normalize(_frustum[3]);

	// ЗАДНЯЯ плоскость
	_frustum[4].x = clip[3] - clip[2];
	_frustum[4].y = clip[7] - clip[6];
	_frustum[4].z = clip[11] - clip[10];
	_frustum[4].w = clip[15] - clip[14];
	normalize(_frustum[4]);

	// ПЕРЕДНЯЯ плоскость
	_frustum[5].x = clip[3] + clip[2];
	_frustum[5].y = clip[7] + clip[6];
	_frustum[5].z = clip[11] + clip[10];
	_frustum[5].w = clip[15] + clip[14];
	normalize(_frustum[5]);
}

// PUBLIC

void Camera::setDefault()
{
	_matProject = perspective(45.0f, Engine::Screen::aspect(), 0.1f, 1000.0f);
	setLookAt(vec3(25.0f, 25.0f, 25.0f), vec3(0.0f, 0.0f, 0.0f));
}

const float* Camera::matPVM(const mat4x4 &matModel)
{
	_matPVM = _matProjectView * matModel;
	return value_ptr(_matPVM);
}

float Camera::frustum(const mat4x4 &mat, const float &radius)
{
	float dist = FLT_MAX;
	if (!_calcFrustum) return dist;

	for (int i = 0; i < 6; ++i)
	{
		dist = _frustum[i].x * mat[3][0] + _frustum[i].y * mat[3][1] + _frustum[i].z * mat[3][2] + _frustum[i].w;
		if (dist <= -radius) return 0.0f;
	}
	return dist + radius;
}

void Camera::setOrtho(const float &left, const float &right, const float &bottom, const float &top)
{
	_matProject = ortho(left, right, bottom, top);
	makeMatProjectView();
}

void Camera::setPerspective(const float &fov, const float &aspect, const float &zNear, const float &zFar)
{
	_matProject = perspective(fov, aspect, zNear, zFar);
	makeMatProjectView();
}

void Camera::setLookAt(const vec3 &eye, const vec3 &center)
{
	if (_fromEye)
	{
		_pos = eye;
		_vector = center - eye;
		_dist = length(_vector);
		_vector = normalize(_vector);
	}
	else
	{
		_pos = center;
		_vector = center - eye;
		_dist = length(_vector);
		_vector = normalize(_vector);
	}

	_matView = lookAt(eye, center, vec3(0.0f, 0.0f, 1.0f));
	makeMatProjectView();
}

void Camera::setLookAt(const vec3 &pos, const vec3 &vector, const float &dist)
{
	if (_vector != vector || (dist != 0.0f && _dist != dist))
	{
		_pos = pos;
		_vector = vector;

		vec3 eye;
		vec3 center;

		if (_fromEye)
		{
			eye = _pos;
			if (dist > 0.1f) _dist = dist;
			center = _pos + _vector * _dist;
		}
		else
		{
			center = _pos;
			if (dist > 0.1f) _dist = dist;
			eye = _pos + _vector * (-_dist);
		}

		_matView = lookAt(eye, center, vec3(0.0f, 0.0f, 1.0f));
		makeMatProjectView();
	}
	else if (_pos != pos)
	{
		_pos = pos;
		_matView = translate(_matView, _pos);
		makeMatProjectView();
	}
}

void Camera::setCalcFrustum(const bool &calcFrustum)
{
	_calcFrustum = calcFrustum;
	if (_calcFrustum) makeFrustum();
};

void Camera::setFromEye(const bool &fromEye)
{
	if (_fromEye == fromEye) return;

	_fromEye = fromEye;
}

void Camera::setDist(const float &dist)
{
	if (_fromEye) return;
	if (_dist == dist || dist <= 1.0f) return;

	_dist = dist;

	vec3 center = _pos;
	vec3 eye = _pos + _vector * (-_dist);

	_matView = lookAt(eye, center, vec3(0.0f, 0.0f, 1.0f));
	makeMatProjectView();
}

void Camera::setVector(const vec3 &vector)
{
	if (_vector == vector) return;
	_vector = vector;

	vec3 eye;
	vec3 center;

	if (_fromEye)
	{
		eye = _pos;
		center = _pos + _vector * _dist;
	}
	else
	{
		center = _pos;
		eye = _pos + _vector * (-_dist);
	}

	_matView = lookAt(eye, center, vec3(0.0f, 0.0f, 1.0f));
	makeMatProjectView();
}

void Camera::setPos(const vec3 &pos)
{
	if (_pos == pos) return;
	_pos = pos;

	vec3 eye;
	vec3 center;

	if (_fromEye)
	{
		eye = _pos;
		center = _pos + _vector * _dist;
	}
	else
	{
		center = _pos;
		eye = _pos + _vector * (-_dist);
	}

	_matView = lookAt(eye, center, vec3(0.0f, 0.0f, 1.0f));
	makeMatProjectView();
}

void Camera::move(const int &direct, const float speed)
{
	if (speed > 0.0f) _speed = speed;
	_speed = _fromEye ? -_speed : _speed;
	vec3 pos = _pos;

	switch (direct)
	{
	case CAMERA_FORVARD:
		pos -= _vector * speed;
		break;

	case CAMERA_BACK:
		pos += _vector * speed;
		break;

	case CAMERA_LEFT:
		pos.x -= (_vector.y * speed);
		pos.y += (_vector.x * speed);
		break;

	case CAMERA_RIGHT:
		pos.x += (_vector.y * speed);
		pos.y -= (_vector.x * speed);
		break;

	case CAMERA_TOP:
		pos.z += abs(speed);
		break;

	case CAMERA_DOWN:
		pos.z -= abs(speed);
		break;

	case CAMERA_HORIZONT:
		pos.x += (_vector.x * speed);
		pos.y += (_vector.y * speed);
		break;

	case CAMERA_BACK_HORIZONT:
		pos.x -= (_vector.x * speed);
		pos.y -= (_vector.y * speed);
		break;
	}

	setPos(pos);
}

void Camera::move(const vec2 &direct)
{
	float speed = _fromEye ? -_speed : _speed;
	vec3 pos = _pos;

	pos.x += (_vector.y * direct.x * speed);
	pos.y -= (_vector.x * direct.x * speed);

	pos.x -= (_vector.x * direct.y * speed);
	pos.y -= (_vector.y * direct.y * speed);

	setPos(pos);
}

void Camera::rotate(const vec2 &angles)
{
	vec3 vector = _vector;
	double angleY = asinf(vector.z);
	double angleX = acosf(vector.y / static_cast<float>(cos(angleY)));

	if (vector.x < 0.0f)
	{
		angleX = M_PI + (M_PI - angleX);
	}

	angleX = angleX - static_cast<double>(angles.x) * _speedRotate;
	angleY = angleY + static_cast<double>(angles.y) * _speedRotate;

	if (angleY >(M_PI / 2.0 - 0.25)) angleY = (M_PI / 2.0 - 0.25);
	if (angleY < -(M_PI / 2.0 - 0.25)) angleY = -(M_PI / 2.0 - 0.25);

	vector.x = static_cast<float>(sin(angleX) * cos(angleY));
	vector.y = static_cast<float>(cos(angleX) * cos(angleY));
	vector.z = static_cast<float>(sin(angleY));
	normalize(vector);

	setVector(vector);
}

void Camera::setJsonData(const Json::Value& data)
{
	_calcFrustum = data["calcFrustum"].asBool();
	_fromEye = data["fromEye"].asBool();
	_dist = data["dist"].asFloat();

	_pos.x = data["pos"][0].asFloat();
	_pos.y = data["pos"][1].asFloat();
	_pos.z = data["pos"][2].asFloat();

	_vector.x = data["vector"][0].asFloat();
	_vector.y = data["vector"][1].asFloat();
	_vector.z = data["vector"][2].asFloat();

	_speed = data["speed"].asFloat();
	_speedRotate = data["speedRotate"].asFloat();

	//---

	vec3 eye;
	vec3 center;

	if (_fromEye)
	{
		eye = _pos;
		center = _pos + _vector * _dist;
	}
	else
	{
		center = _pos;
		eye = _pos + _vector * (-_dist);
	}

	_matView = lookAt(eye, center, vec3(0.0f, 0.0f, 1.0f));
	makeMatProjectView();
}

void Camera::getJsonData(Json::Value& data)
{
	data["calcFrustum"] = _calcFrustum;
	data["fromEye"] = _fromEye;
	data["dist"] = _dist;

	data["pos"].append(_pos.x);
	data["pos"].append(_pos.y);
	data["pos"].append(_pos.z);

	data["vector"].append(_vector.x);
	data["vector"].append(_vector.y);
	data["vector"].append(_vector.z);

	data["speed"] = _speed;
	data["speedRotate"] = _speedRotate;
}

glm::vec3 Camera::corsorCoord()
{
	glm::vec2 mousePos = Engine::Callback::mousePos();
	glm::vec3 wincoord = glm::vec3(mousePos.x, (Engine::Screen::height() - mousePos.y), 1.0f);
	glm::vec4 viewport = glm::vec4(0, 0, Engine::Screen::width(), Engine::Screen::height());

	glm::vec3 coord = glm::unProject(wincoord, _matView, _matProject, viewport);

	glm::vec3 eye = _pos;
	vec3 vecCursor(eye.x - coord.x, eye.y - coord.y, eye.z - coord.z);
	vecCursor = normalize(vecCursor);

	PhysicPlane plane;
	plane.set(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));

	vec3 objcoord = plane.crossVector(vecCursor, eye);

	return objcoord;
}

//---------------------------------------------------------------------------------------------

mat4x4 Camera::_matPVM;
Camera Camera::current;

Camera& Camera::setCurrent(Camera& camera)
{
	current = camera;
	return current;
}

Camera& Camera::getCurrent() {
	return current;
}
