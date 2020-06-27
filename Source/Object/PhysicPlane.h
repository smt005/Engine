#pragma once

using namespace std;

#include "glm/vec3.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class PhysicPlane
{
private:
public:
	vec3 _p1, _p2, _p3;
	float a, b, c, d;
	vec3 _normal;
	float bottomValue;
	float _area;

	const float _error = 0.1f;

public:
	inline void set(const vec3& p1, const vec3& p2, const vec3& p3) {
		set(glm::value_ptr(p1), glm::value_ptr(p2), glm::value_ptr(p3));
	}

	inline float dist(const float* pos)	{
		return (a * pos[0] + b * pos[1] + c * pos[2]) / bottomValue;
	}

	inline float distByVector(const vec3& vector, const vec3& pos = vec3(0.0f))	{
		return distByVector(value_ptr(vector), value_ptr(pos));
	}

	inline float distByVector(const float* vector, const float* pos) {
		return -(a * pos[0] + b * pos[1] + c * pos[2] + d) / (a * vector[0] + b * vector[1] + c * vector[2]);
	}

	inline float distPointToPlane(const vec3& pos) {
		float dist = (a * pos.x + b * pos.y + c * pos.z + d) / bottomValue;
		return dist;
	}

	inline void areaTriangle() {
		_area = areaTriangle(_p1, _p2, _p3);
		_area += _area * _error;
	}

	inline float areaTriangle(const vec3& p1, const vec3& p2, const vec3& p3)
	{
		float AB = length(p1 - p2);
		float BC = length(p2 - p3);
		float CA = length(p3 - p1);

		float p = (AB + BC + CA) / 2.0f;
		float area = sqrt(p*(p - AB) * (p - BC) * (p - CA));

		return area;
	}

	void set(const float* p1, const float* p2, const float* p3);
	vec3 crossVector(const vec3& vector, const vec3& pos = vec3(0.0f));
	float entryPointToPlane(const vec3& pos, const float dist, const float error = 0.0f);
	bool entryPointToPlane(vec3& pos);
	vec3 reflect(const vec3& vectorTarget, const float elasticity = 1.0f);

	//help

	inline static float lenght(const float* point) {
		return sqrt(pow(point[0], 2) + pow(point[1], 2) + pow(point[2], 2));
	}

	inline static float lenght(const glm::vec3& point) {
		return sqrt(pow(point[0], 2) + pow(point[1], 2) + pow(point[2], 2));
	}

	inline static float maxLenght(const float* point0, const float* point1, const float* point2)
	{
		float l0 = lenght(point0);
		float l1 = lenght(point1);
		float l2 = lenght(point2);
		float l = 0.0f;

		l0 = l0 > l1 ? l0 : l1;
		l0 = l2 > l0 ? l2 : l0;

		return l0;
	}

	inline static float dist(const glm::vec3& pos0, const glm::vec3& pos1) {
		glm::vec3 vec(pos1 - pos0);
		return glm::length(vec);
	}
};
