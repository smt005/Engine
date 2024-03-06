#include "Plane.h"

void Plane::Make(const Math::Vector3& min, const Math::Vector3& max) {
	unsigned int countVertex = 4;
	unsigned int countIndex = 6;

	float* aVertex = new float[countVertex * 3](0.f);
	if (!aVertex) {
		return;
	}

	unsigned int* aIndex = new unsigned int[countIndex];
	if (!aIndex) {
		delete[] aVertex;
		aVertex = nullptr;
	}

	float halfZ = min.z + (max.z - min.z) * 0.5f;

	aVertex[0] = min.x;
	aVertex[1] = min.y;
	aVertex[2] = min.z;

	aVertex[3] = max.x;
	aVertex[4] = max.y;
	aVertex[5] = max.z;

	aVertex[6] = min.x;
	aVertex[7] = max.y;
	aVertex[8] = halfZ;

	aVertex[9] = max.x;
	aVertex[10] = min.y;
	aVertex[11] = halfZ;

	aIndex[0] = 0;
	aIndex[1] = 1;
	aIndex[2] = 2;
	aIndex[3] = 1;
	aIndex[4] = 0;
	aIndex[5] = 3;

	setData(aVertex,
		nullptr,
		nullptr,
		countVertex,
		aIndex,
		countIndex);
}
