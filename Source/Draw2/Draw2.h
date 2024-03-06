#pragma once

#include <glm/mat4x4.hpp>

class Mesh;
class Model;

class Draw2 final {
public:
	static void SetClearColor(float r, float g, float b, float a);
	static void ClearColor();
	static void ClearDepth();
	static void DepthTest(bool enable);
	static void Viewport();
	static void SetModelMatrix(const glm::mat4x4& matrix);
	static void SetModelMatrix(const unsigned int u_matViewModel, const glm::mat4x4& matrix);
	static void SetPointSize(const float sizePoint);
	static void SetColor(const unsigned int u_color, const float* const color);

	template<typename ShaderT>
	static void SetModelMatrixClass(const glm::mat4x4& matrix) {
		SetModelMatrix(ShaderT::u_matViewModel, matrix);
	}

	template<typename ShaderT>
	static void SetColorClass(const float* const color) {
		SetColor(ShaderT::u_color, color);
	}

	static void Draw(Mesh& shape);
	static void Lines(Mesh& shape);
	static void Draw(Model& model);

	static void drawPoints(const float* vertices, const unsigned int count);

public:
	static unsigned int u_matViewModel;

private:
	static unsigned int currentVAO;
	static unsigned int currentTexture;
};
