
#include "UIElement.h"
#include <functional>
#include "Screen.h"
#include "Draw/Camera.h"
#include "Common/Help.h"

namespace UI {
	Element::Element()
		: _count(0)
		, _triangles(nullptr) {

	}

	Element::~Element() {
		if (_triangles) {
			delete[] _triangles;
		}
	}

	bool Element::hit() {
		glm::vec2 mousePos = Engine::Callback::mousePos();
		return hit(mousePos);
	}

	/*bool Element::hint(const glm::vec2& point) {
		if (!_triangles || !_count)
			return false;

		glm::vec3 maxVectex(-10000);
		glm::vec3 minVectex(+10000);

		for (int iT = 0; iT < _count; ++iT) {
			for (unsigned short int i = 0; i < _triangles[iT].countVertex(); ++i) {
				float x = _triangles[iT].aVertex()[0];
				float y = _triangles[iT].aVertex()[1];
				float z = _triangles[iT].aVertex()[2];

				if (x > maxVectex.x)
					maxVectex.x = x;
				if (y > maxVectex.y)
					maxVectex.y = y;
				if (z > maxVectex.z)
					maxVectex.z = z;

				if (x < minVectex.x)
					minVectex.x = x;
				if (y < minVectex.y)
					minVectex.y = y;
				if (z < minVectex.z)
					minVectex.z = z;
			}
		}

		std::function<void(glm::vec3&, glm::mat4x4&)> transformToScreen = [](glm::vec3 &point, glm::mat4x4 &mat) {
			glm::vec4 p(point.x, point.y, point.z, 1.0f);
			p = mat * p;

			if (p.w != 1.0)
			{
				p.x /= p.w;
				p.y /= p.w;
				p.z /= p.w;
			}

			point.x = p.x;
			point.y = p.y;
			point.z = p.z;
		};

		std::function<void(float&, float&, int&, int&)> transformSizeToScreen = [](float &x, float &y, int &xRes, int &yRes) {
			x = x * 0.5f + 0.5f;
			y = y * 0.5f + 0.5f;

			xRes = Engine::Screen::width() * x;
			yRes = Engine::Screen::height() * y;
		};

		glm::mat4x4 mat = _triangles[0].getMatrix();
		transformToScreen(maxVectex, mat);
		transformToScreen(minVectex, mat);

		int xMaxInt;
		int yMaxInt;
		int xMinInt;
		int yMinInt;

		transformSizeToScreen(maxVectex.x, maxVectex.y, xMaxInt, yMaxInt);
		transformSizeToScreen(minVectex.x, minVectex.y, xMinInt, yMinInt);

		std::string  logStr(" [" + std::to_string(xMaxInt) + " > " + std::to_string(point.x) + "] && [" + std::to_string(point.x) + " > " + std::to_string(xMinInt) + "] && [" + std::to_string(yMaxInt) + " > " + std::to_string(point.y) + "] && [" + std::to_string(point.y) + " > " + std::to_string(yMinInt) + "] ");
		help::log(logStr);

		if ((xMaxInt > point.x && point.x > xMinInt && yMaxInt > point.y && point.y > yMinInt)) {
			return true;
		}

		return false;
	}*/

	bool Element::hit(const glm::vec2& mousePos) {
		std::function<void(glm::vec3&, glm::mat4x4&)> transformToScreen = [](glm::vec3 &point, const glm::mat4x4 &mat) {

			glm::vec3 pointTemp = point;

			glm::vec4 p(point.x, point.y, point.z, 1.0f);
			p = mat * p;

			if (p.w != 1.0)
			{
				p.x /= p.w;
				p.y /= p.w;
				p.z /= p.w;
			}

			point.x = p.x;
			point.y = p.y;
			point.z = p.z;

			std::string logStr("transformToScreen: [" + std::to_string(pointTemp.x) + ", " + std::to_string(pointTemp.y) + ", " + std::to_string(pointTemp.z) + "] [" + std::to_string(point.x) + ", " + std::to_string(point.y) + ", " + std::to_string(point.z) + ']');
			help::log(logStr);
		};

		std::function<void(float&, float&, int&, int&)> transformSizeToScreen = [](float &x, float &y, int &xRes, int &yRes) {
			x = x * 0.5f + 0.5f;
			y = y * 0.5f + 0.5f;

			xRes = Engine::Screen::width() * x;
			yRes = Engine::Screen::height() * y;
		};

		float xMax = FLT_MIN;
		float xMin = FLT_MAX;

		float yMax = FLT_MIN;
		float yMin = FLT_MAX;

		float zMax = FLT_MIN;
		float zMin = FLT_MAX;

		Triangle& triangle = _triangles[0];
		int index = 0;
		for (int i = 0; i < triangle.countVertex(); ++i)
		{
			glm::vec3 point(triangle.aVertex()[index], triangle.aVertex()[index + 1], triangle.aVertex()[index + 2]);
			index += 3;

			glm::mat4x4 mat = _triangles[0].getMatrix();
			transformToScreen(point, mat);

			float x = point.x;
			if (x > xMax) xMax = x;
			if (x < xMin) xMin = x;

			float y = point.y;
			if (y > yMax) yMax = y;
			if (y < yMin) yMin = y;

			float z = point.z;
			if (z > zMax) zMax = z;
			if (z < zMin) zMin = z;
		}

		int xMaxInt;
		int yMaxInt;
		int xMinInt;
		int yMinInt;

		transformSizeToScreen(xMax, yMax, xMaxInt, yMaxInt);
		transformSizeToScreen(xMin, yMin, xMinInt, yMinInt);

		std::string  logStr(" [" + std::to_string(xMaxInt) + " > " + std::to_string(mousePos.x) + "] && [" + std::to_string(mousePos.x) + " > " + std::to_string(xMinInt) + "] && [" + std::to_string(yMaxInt) + " > " + std::to_string(mousePos.y) + "] && [" + std::to_string(mousePos.y) + " > " + std::to_string(yMinInt) + "] ");
		help::log(logStr);

		if ((xMaxInt > mousePos.x && mousePos.x > xMinInt && yMaxInt > mousePos.y && mousePos.y > yMinInt)) {
			return true;
		}

		return false;
	}

	void Element::set(const int count, Triangle* triangle, TexturePtr texturePtr) {

	}

	bool Element::load(const std::string& name) {
		if (!UI::UIData::load(dataPtr, name)) {
			return false;
		}


		return false;
	}

	ElementPtr Element::make(const UIDataPtr& dataPtr, const std::string& name) {
		return nullptr;
	}

	ElementPtr Element::make(const glm::vec2& size, const std::string& texteruName, const std::string& name) {
		Element* element = new Element();
		ElementPtr elementPtr(element);

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

		element->_count = 1;
		element->_triangles = new Triangle[1];
		element->_triangles[0].setData(Triangle::STRIP, countPoints, points, texCoord);
		element->_triangles[0].setTexture(texteruName);

		return elementPtr;
	}
}
