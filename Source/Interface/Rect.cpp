
#include "Interface/Rect.h"

Rect::Rect() {
	init(1);
}

Rect::Rect(const unsigned int countPoligon) {
	init(countPoligon);
}

void Rect::init(const unsigned int countPoligon) {
	switch (countPoligon)
	{
	case 1: {
		const unsigned int countPoints = 4;
		float height = 0.0f;
		Triangle::Point* points = new Triangle::Point[countPoints];
		points[0].data[0] = 0.0f;	points[0].data[1] = 1.0f;	points[0].data[2] = height;
		points[1].data[0] = 0.0f;	points[1].data[1] = 0.0f;	points[1].data[2] = height;
		points[2].data[0] = 1.0f;	points[2].data[1] = 1.0f;	points[2].data[2] = height;
		points[3].data[0] = 1.0f;	points[3].data[1] = 0.0f;	points[3].data[2] = height;

		Triangle::TexCoord* texCoord = new Triangle::TexCoord[countPoints];
		texCoord[0].data[0] = 0.0f;	texCoord[0].data[1] = 1.0f;
		texCoord[1].data[0] = 0.0f;	texCoord[1].data[1] = 0.0f;
		texCoord[2].data[0] = 1.0f;	texCoord[2].data[1] = 1.0f;
		texCoord[3].data[0] = 1.0f;	texCoord[3].data[1] = 0.0f;

		_triangle.setData(Triangle::STRIP, countPoints, points, texCoord);

		_triangle.setTexture("Test/strip_1.png");
	} break;
	case 2: {
		const unsigned int countPoints = 8;
		float height = 0.0f;
		Triangle::Point* points = new Triangle::Point[countPoints];
		points[0].data[0] = 0.0f;	points[0].data[1] = 1.0f;	points[0].data[2] = height;
		points[1].data[0] = 0.0f;	points[1].data[1] = 0.0f;	points[1].data[2] = height;
		points[2].data[0] = 1.0f;	points[2].data[1] = 1.0f;	points[2].data[2] = height;
		points[3].data[0] = 1.0f;	points[3].data[1] = 0.0f;	points[3].data[2] = height;
		points[4].data[0] = 2.0f;	points[4].data[1] = 1.0f;	points[4].data[2] = height;
		points[5].data[0] = 2.0f;	points[5].data[1] = 0.0f;	points[5].data[2] = height;
		points[6].data[0] = 3.0f;	points[6].data[1] = 1.0f;	points[6].data[2] = height;
		points[7].data[0] = 3.0f;	points[7].data[1] = 0.0f;	points[7].data[2] = height;

		float var1_3x1 = 1.0f / 3.0f;
		float var1_3x2 = var1_3x1 * 2.0f;

		Triangle::TexCoord* texCoord = new Triangle::TexCoord[countPoints];
		texCoord[0].data[0] = 0.0f;		texCoord[0].data[1] = 1.0f;
		texCoord[1].data[0] = 0.0f;		texCoord[1].data[1] = 0.0f;
		texCoord[2].data[0] = var1_3x1;	texCoord[2].data[1] = 1.0f;
		texCoord[3].data[0] = var1_3x1;	texCoord[3].data[1] = 0.0f;
		texCoord[4].data[0] = var1_3x2;	texCoord[4].data[1] = 1.0f;
		texCoord[5].data[0] = var1_3x2;	texCoord[5].data[1] = 0.0f;
		texCoord[6].data[0] = 1.0f;		texCoord[6].data[1] = 1.0f;
		texCoord[7].data[0] = 1.0f;		texCoord[7].data[1] = 0.0f;

		_triangle.setData(Triangle::STRIP, countPoints, points, texCoord);

		_triangle.setTexture("Test/strip_2.png");
	} break;
	case 3: {
		const unsigned int countPoints = 12;
		float height = 0.0f;
		Triangle::Point* points = new Triangle::Point[countPoints];
		points[0].data[0] = 0.0f;	points[0].data[1] = 1.0f;	points[0].data[2] = height;
		points[1].data[0] = 0.0f;	points[1].data[1] = 0.0f;	points[1].data[2] = height;
		points[2].data[0] = 1.0f;	points[2].data[1] = 1.0f;	points[2].data[2] = height;
		points[3].data[0] = 1.0f;	points[3].data[1] = 0.0f;	points[3].data[2] = height;
		points[4].data[0] = 2.0f;	points[4].data[1] = 1.0f;	points[4].data[2] = height;
		points[5].data[0] = 2.0f;	points[5].data[1] = 0.0f;	points[5].data[2] = height;
		points[6].data[0] = 3.0f;	points[6].data[1] = 1.0f;	points[6].data[2] = height;
		points[7].data[0] = 3.0f;	points[7].data[1] = 0.0f;	points[7].data[2] = height;
		points[8].data[0] = 4.0f;	points[8].data[1] = 1.0f;	points[8].data[2] = height;
		points[9].data[0] = 4.0f;	points[9].data[1] = 0.0f;	points[9].data[2] = height;
		points[10].data[0] = 5.0f;	points[10].data[1] = 1.0f;	points[10].data[2] = height;
		points[11].data[0] = 5.0f;	points[11].data[1] = 0.0f;	points[11].data[2] = height;

		float var1_5x1 = 1.0f / 5.0f;
		float var1_5x2 = var1_5x1 * 2.0f;
		float var1_5x3 = var1_5x1 * 3.0f;
		float var1_5x4 = var1_5x1 * 4.0f;

		Triangle::TexCoord* texCoord = new Triangle::TexCoord[countPoints];
		texCoord[0].data[0] = 0.0f;			texCoord[0].data[1] = 1.0f;
		texCoord[1].data[0] = 0.0f;			texCoord[1].data[1] = 0.0f;
		texCoord[2].data[0] = var1_5x1;		texCoord[2].data[1] = 1.0f;
		texCoord[3].data[0] = var1_5x1;		texCoord[3].data[1] = 0.0f;
		texCoord[4].data[0] = var1_5x2;		texCoord[4].data[1] = 1.0f;
		texCoord[5].data[0] = var1_5x2;		texCoord[5].data[1] = 0.0f;
		texCoord[6].data[0] = var1_5x3;		texCoord[6].data[1] = 1.0f;
		texCoord[7].data[0] = var1_5x3;		texCoord[7].data[1] = 0.0f;
		texCoord[8].data[0] = var1_5x4;		texCoord[8].data[1] = 1.0f;
		texCoord[9].data[0] = var1_5x4;		texCoord[9].data[1] = 0.0f;
		texCoord[10].data[0] = 1.0f;		texCoord[10].data[1] = 1.0f;
		texCoord[11].data[0] = 1.0f;		texCoord[11].data[1] = 0.0f;

		_triangle.setData(Triangle::STRIP, countPoints, points, texCoord);

		_triangle.setTexture("Test/strip_3.png");
	} break;
	default:
		break;
	}
}

// Rect3

Rect3::Rect3()
	:_triangles(nullptr) {
	init();
}

Rect3::~Rect3() {
	if (_triangles) {
		delete[] _triangles;
	}
}

void Rect3::init() {
	if (_triangles) {
		delete[] _triangles;
	}

	_triangles = new Triangle[3];

	getStrip(_triangles[0], 0.0f, 1.0f / 3.0f, "Test/Plane.png");
	getStrip(_triangles[1], 1.0f / 3.0f, (1.0f / 3.0f) * 2.0f, "Test/Plane.png");
	getStrip(_triangles[2], (1.0f / 3.0f) * 2.0f, 1.0f, "Test/Plane.png");
}

void Rect3::getStrip(Triangle& triangle, const float texCoord_0, const float texCoord_1, const std::string& textureStr) {
	const unsigned int countPoints = 12;
	float height = 0.0f;
	Triangle::Point* points = new Triangle::Point[countPoints];
	points[0].data[0] = 0.0f;	points[0].data[1] = 1.0f;	points[0].data[2] = height;
	points[1].data[0] = 0.0f;	points[1].data[1] = 0.0f;	points[1].data[2] = height;
	points[2].data[0] = 1.0f;	points[2].data[1] = 1.0f;	points[2].data[2] = height;
	points[3].data[0] = 1.0f;	points[3].data[1] = 0.0f;	points[3].data[2] = height;
	points[4].data[0] = 2.0f;	points[4].data[1] = 1.0f;	points[4].data[2] = height;
	points[5].data[0] = 2.0f;	points[5].data[1] = 0.0f;	points[5].data[2] = height;
	points[6].data[0] = 3.0f;	points[6].data[1] = 1.0f;	points[6].data[2] = height;
	points[7].data[0] = 3.0f;	points[7].data[1] = 0.0f;	points[7].data[2] = height;
	points[8].data[0] = 4.0f;	points[8].data[1] = 1.0f;	points[8].data[2] = height;
	points[9].data[0] = 4.0f;	points[9].data[1] = 0.0f;	points[9].data[2] = height;
	points[10].data[0] = 5.0f;	points[10].data[1] = 1.0f;	points[10].data[2] = height;
	points[11].data[0] = 5.0f;	points[11].data[1] = 0.0f;	points[11].data[2] = height;

	float var1_5x1 = 1.0f / 5.0f;
	float var1_5x2 = var1_5x1 * 2.0f;
	float var1_5x3 = var1_5x1 * 3.0f;
	float var1_5x4 = var1_5x1 * 4.0f;

	Triangle::TexCoord* texCoord = new Triangle::TexCoord[countPoints];
	texCoord[0].data[0] = 0.0f;			texCoord[0].data[1] = texCoord_1;
	texCoord[1].data[0] = 0.0f;			texCoord[1].data[1] = texCoord_0;
	texCoord[2].data[0] = var1_5x1;		texCoord[2].data[1] = texCoord_1;
	texCoord[3].data[0] = var1_5x1;		texCoord[3].data[1] = texCoord_0;
	texCoord[4].data[0] = var1_5x2;		texCoord[4].data[1] = texCoord_1;
	texCoord[5].data[0] = var1_5x2;		texCoord[5].data[1] = texCoord_0;
	texCoord[6].data[0] = var1_5x3;		texCoord[6].data[1] = texCoord_1;
	texCoord[7].data[0] = var1_5x3;		texCoord[7].data[1] = texCoord_0;
	texCoord[8].data[0] = var1_5x4;		texCoord[8].data[1] = texCoord_1;
	texCoord[9].data[0] = var1_5x4;		texCoord[9].data[1] = texCoord_0;
	texCoord[10].data[0] = 1.0f;		texCoord[10].data[1] = texCoord_1;
	texCoord[11].data[0] = 1.0f;		texCoord[11].data[1] = texCoord_0;

	triangle.setData(Triangle::STRIP, countPoints, points, texCoord);

	triangle.setTexture(textureStr);
}

bool Rect3::hint() {
	if (_testVar > 3) {
		return true;
	}

	++_testVar;
	return false;
}
