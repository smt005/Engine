#include "PhysicPlane.h"

void PhysicPlane::set(const float* p1, const float* p2, const float* p3)
{
	_p1 = vec3(p1[0], p1[1], p1[2]);
	_p2 = vec3(p2[0], p2[1], p2[2]);
	_p3 = vec3(p3[0], p3[1], p3[2]);

	float x1 = p1[0];
	float y1 = p1[1];
	float z1 = p1[2];

	float x2 = p2[0];
	float y2 = p2[1];
	float z2 = p2[2];

	float x3 = p3[0];
	float y3 = p3[1];
	float z3 = p3[2];

	a = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	b = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	c = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

	d = x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1);
	d = -d;

	bottomValue = pow(a, 2) + pow(b, 2) + pow(c, 2);
	bottomValue = sqrt(bottomValue);

	if (bottomValue == 0.0f)
		bottomValue = 0.000001f;

	_normal = normalize(vec3(a, b, c));

	areaTriangle();
}

vec3 PhysicPlane::crossVector(const vec3& vector, const vec3& pos)
{
	float dist = distByVector(vector, pos);

	return vec3(pos.x + vector.x * dist,
		pos.y + vector.y * dist,
		pos.z + vector.z * dist);
}

float PhysicPlane::entryPointToPlane(const vec3& pos, const float dist, const float error)
{
	float areaTriangles = 0.0f;

	vec3 posOnPlane(pos);
	vec3 vec(_normal * dist);
	posOnPlane -= vec;

	areaTriangles += areaTriangle(_p1, _p2, posOnPlane);
	areaTriangles += areaTriangle(_p2, _p3, posOnPlane);
	areaTriangles += areaTriangle(_p3, _p1, posOnPlane);

	return areaTriangles <= (_area + error) ? true : false;
}

bool PhysicPlane::entryPointToPlane(vec3& pos)
{
	float dist = distPointToPlane(pos);

	if (dist > 0)
	{
		//_CrtDbgReport(0, NULL, 0, NULL, "LOG: dist: %f = %s\n", dist, "NO");
		return false;
	}

	vec3 posOnPlane(pos);
	vec3 vec(_normal * dist);
	posOnPlane -= vec;

	float areaTriangles = 0.0f;
	areaTriangles += areaTriangle(_p1, _p2, posOnPlane);
	areaTriangles += areaTriangle(_p2, _p3, posOnPlane);
	areaTriangles += areaTriangle(_p3, _p1, posOnPlane);

	pos = posOnPlane;

	bool res = areaTriangles <= _area ? true : false;
	//_CrtDbgReport(0, NULL, 0, NULL, "LOG: dist: %f - %fm %f = %s\n", dist, areaTriangles,  _area, (res ? "yes" : "NO"));
	return res;
	//		return areaTriangles <= _area ? true : false;
}

vec3 PhysicPlane::reflect(const vec3& vectorTarget, const float elasticity)
{
	//if (length(vectorTarget) == 0.0f)
	if (length(vectorTarget) < 0.01f)
		return vec3(0.0f);

	float speed = length(vectorTarget);
	vec3 vector = normalize(vectorTarget);

	float dot = glm::dot(_normal, vector);
	vec3 valueVector = _normal;
	valueVector *= dot;
	valueVector *= (-2.0f);
	vec3 v(vector + valueVector);
	v *= elasticity;
	v *= speed;

	return v;
}
