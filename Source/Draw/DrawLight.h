
#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include "../../Engine/Source/Object/Model.h"

class Mesh;
//class Model;
class Object;
class Map;
class Triangle;

class DrawLight
{
public:
	static void setClearColor(const float r, const float g, const float b, const float a);
	static const float * const getClearColor();
	static void setLightPos(const float x, const float y, const float z);

	static void clearColor();
	static void viewport();
	static void prepare();
	static void resetShader();

	static void draw(Mesh& mesh);
	static void draw(Model& model);
	static void draw(Object& object);
	static void draw(Map& map);
	static void draw(const Triangle& triangle);

	template<typename MapT>
	static void DrawMap(MapT& map) {
		for (auto&& object : map.Objects()) {
			DrawObject(*object);
		}
	}

	template<typename ObjectT>
	static void DrawObject(ObjectT& object) {
		SetMatrux(object.getMatrix());

		Model& model = object.getModel();
		draw(model);
	}

	static void SetMatrux(const glm::mat4x4& matrix);

private:
	static float _clearColor[4];
	static float _lightPos[4];
	static unsigned int curentBufer;
	static unsigned int currentTexture;
};
