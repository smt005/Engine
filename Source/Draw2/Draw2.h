#pragma once

#include <glm/mat4x4.hpp>

class Mesh;
class Model;

class Draw2 final {
public:
	static void SetClearColor(float r, float g, float b, float a);
	static void ClearColor();
	static void ClearDepth();
	static void Viewport();
	static void SetModelMatrix(const glm::mat4x4& matrix);

	static void Draw(Mesh& shape);
	static void Draw(Model& model);

private:
	static unsigned int currentVAO;
	static unsigned int currentTexture;
};
