#pragma once

#include <functional>
#include <glm/mat4x4.hpp>

class Mesh;
class Model;

enum class CullFaceType {
	NONE,
	FRONT,
	BACK,
	FRONT_AND_BACK,
	FRONT_LEFT,
	FRONT_RIGHT
};

class Draw2 final {
public:
	static void SetClearColor(float r, float g, float b, float a);
	static void ClearColor();
	static void ClearDepth();
	static void DepthTest(bool enable);
	static void CullFace(CullFaceType type);
	static void Viewport();
	static void SetModelMatrix(const glm::mat4x4& matrix);
	static void SetModelMatrix(const unsigned int u_matViewModel, const glm::mat4x4& matrix);
	static void SetPointSize(const float sizePoint);
	static void SetColor(const unsigned int u_color, const float* const color);
	static void SetUniform1f(const unsigned int uniform, const float value);
	static void SetUniform1fv(const unsigned int uniform, const float* color, int count = 1);
	static void SetUniform2fv(const unsigned int uniform, const float* color, int count = 1);
	static void SetUniform3fv(const unsigned int uniform, const float* color, int count = 1);
	static void SetUniform4fv(const unsigned int uniform, const float* color, int count = 1);

	static void SetUniform1i(const unsigned int uniform, const int value);

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
	static void drawLines(const float* vertices, const unsigned int count);

	static void AddFunction(std::function<bool()> fun);
	static void DrawFunctions(std::function<bool()> fun = nullptr);

public:
	static unsigned int u_matViewModel;

private:
	static unsigned int currentVAO;
	static unsigned int currentTexture;
	static std::vector<std::function<bool()>> _functions;
};
