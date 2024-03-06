#include "Box.h"
#include <stdio.h>

void Box::Make(const Math::Vector3& min, const Math::Vector3& max) {
	unsigned int countVertex = 8;
	unsigned int countIndex = 36;

	float* aVertex = new float[countVertex * 3](0.f);
	if (!aVertex) {
		return;
	}

	unsigned int* aIndex = new unsigned int[countIndex];
	if (!aIndex) {
		delete[] aVertex;
		aVertex = nullptr;
	}

	aVertex[0] = min.x;
	aVertex[1] = min.y;
	aVertex[2] = min.z;
	aVertex[3] = min.x;
	aVertex[4] = max.y;
	aVertex[5] = min.z;
	aVertex[6] = max.x;
	aVertex[7] = max.y;
	aVertex[8] = min.z;
	aVertex[9] = max.x;
	aVertex[10] = min.y;
	aVertex[11] = min.z;
	aVertex[12] = min.x;
	aVertex[13] = min.y;
	aVertex[14] = max.z;
	aVertex[15] = max.x;
	aVertex[16] = min.y;
	aVertex[17] = max.z;
	aVertex[18] = max.x;
	aVertex[19] = max.y;
	aVertex[20] = max.z;
	aVertex[21] = min.x;
	aVertex[22] = max.y;
	aVertex[23] = max.z;

	aIndex[0] = 1;
	aIndex[1] = 2;
	aIndex[2] = 3;
	aIndex[3] = 3;
	aIndex[4] = 4;
	aIndex[5] = 1;
	aIndex[6] = 5;
	aIndex[7] = 6;
	aIndex[8] = 7;
	aIndex[9] = 7;
	aIndex[10] = 8;
	aIndex[11] = 5;
	aIndex[12] = 1;
	aIndex[13] = 4;
	aIndex[14] = 6;
	aIndex[15] = 6;
	aIndex[16] = 5;
	aIndex[17] = 1;
	aIndex[18] = 4;
	aIndex[19] = 3;
	aIndex[20] = 7;
	aIndex[21] = 7;
	aIndex[22] = 6;
	aIndex[23] = 4;
	aIndex[24] = 3;
	aIndex[25] = 2;
	aIndex[26] = 8;
	aIndex[27] = 8;
	aIndex[28] = 7;
	aIndex[29] = 3;
	aIndex[30] = 2;
	aIndex[31] = 1;
	aIndex[32] = 5;
	aIndex[33] = 5;
	aIndex[34] = 8;
	aIndex[35] = 2;

	for (int i = 0; i < 36; ++i) {
		aIndex[i] = aIndex[i] - 1;
	}

	setData(aVertex,
		nullptr,
		nullptr,
		countVertex,
		aIndex,
		countIndex);
}
