#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json/json.h>

using namespace glm;

#define CAMERA_FORVARD			1
#define CAMERA_BACK				2
#define CAMERA_LEFT				3
#define CAMERA_RIGHT			4
#define CAMERA_TOP				5
#define CAMERA_DOWN				6
#define CAMERA_HORIZONT			7
#define CAMERA_BACK_HORIZONT	8

class Camera
{
public:
	typedef std::shared_ptr<Camera> Ptr;

private:
	mat4x4 _matProject;
	mat4x4 _matView;
	mat4x4 _matProjectView;
	vec4 _frustum[6];

	bool _calcFrustum = false;
	bool _fromEye = true;
	float _dist = 10.0f;
	vec3 _pos;
	vec3 _vector;
	float _speed = 0.00001f;
	float _speedRotate = 0.01f;

	void makeMatProjectView();
	void makeFrustum();

public:
	Camera();
	~Camera();
	void setDefault();

	const mat4x4& matProject() { return _matProject; }
	const mat4x4& matView() { return _matView; }
	const mat4x4& matProjectView() { return _matProjectView; }
	const float* matProjectViewFloat() { return value_ptr(_matProjectView); }
	const float* matP() { return value_ptr(_matProject); }
	const float* matPV() { return value_ptr(_matProjectView); }
	const float* matPVM(const mat4x4 &matModel);

	float frustum(const mat4x4 &mat, const float &radius);

	void setOrtho(const float &left, const float &right, const float &bottom, const float &top);
	void setPerspective(const float &fov, const float &aspect, const float &zNear, const float &zFar);
	void setLookAt(const vec3 &eye, const vec3 &center);
	void setLookAt(const vec3 &pos, const vec3 &vector, const float &dist);

	const bool& calcFrustum() { return _calcFrustum; }
	const bool& fromEye() { return _fromEye; }
	const float& dist() { return _dist; }
	const vec3& pos() { return _pos; }
	const vec3& vector() { return _vector; }
	const float& speed() { return _speed; }
	const float& speedRotate() { return _speedRotate; }

	void setCalcFrustum(const bool &calcFrustum);
	void setFromEye(const bool &fromEye);
	void setDist(const float &dist);
	void setVector(const vec3 &vector);
	void setPos(const vec3 &pos);
	void setSpeed(const float &speed) { _speed = speed; };
	void setSpeedRotate(const float &speedRotate) { _speedRotate = speedRotate; };

	void move(const int &direct, const float speed = 0.0f);
	void move(const vec2 &direct);
	void rotate(const vec2 &angles);
	
	void setJsonData(const Json::Value& data);
	void getJsonData(Json::Value& data);
	
	glm::vec3 corsorCoord();

	// STATIC
	static mat4x4 _matPVM;
	static Camera current;
	static Camera& setCurrent(const Camera& camera);
	static Camera& getCurrent();

};