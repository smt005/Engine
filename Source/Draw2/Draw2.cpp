#include "Draw2/Draw2.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <Screen.h>
#include <Draw2/Shader/ShaderLine.h>
#include <Draw2/Shader/ShaderDefault.h>

#include <Object/Shape.h>
#include <Object/Model.h>

unsigned int Draw2::u_matViewModel = 0;
unsigned int Draw2::currentVAO = 0;
unsigned int Draw2::currentTexture = 0;

void Draw2::SetClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void Draw2::ClearColor() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw2::ClearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Draw2::DepthTest(bool enable) {
	if (enable) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}

void Draw2::CullFace(CullFaceType type) {
	if (type == CullFaceType::NONE) {
		glDisable(GL_CULL_FACE);
		return;
	}

	glEnable(GL_CULL_FACE);
	
	if (type == CullFaceType::FRONT) {
		glCullFace(GL_FRONT);
	}
	else if (type == CullFaceType::BACK) {
		glCullFace(GL_BACK);
	}
	else if (type == CullFaceType::FRONT_AND_BACK) {
		glCullFace(GL_FRONT_AND_BACK);
	}
	else if (type == CullFaceType::FRONT_LEFT) {
		glCullFace(GL_FRONT_LEFT);
	}
	else if (type == CullFaceType::FRONT_RIGHT) {
		glCullFace(GL_FRONT_RIGHT);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
}

void Draw2::Viewport() {
	int widthScreen = Engine::Screen::width();
	int heightScreen = Engine::Screen::height();
	glViewport(0, 0, widthScreen, heightScreen);
}

void Draw2::SetModelMatrix(const glm::mat4x4& matrix) {
	glUniformMatrix4fv(ShaderDefault::u_matViewModel, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Draw2::SetModelMatrix(const unsigned int u_matViewModel, const glm::mat4x4& matrix) {
	glUniformMatrix4fv(u_matViewModel, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Draw2::SetPointSize(const float sizePoint) {
	if (sizePoint >= 0.f) {
		glPointSize(sizePoint);
	}
	else {
		glEnable(GL_PROGRAM_POINT_SIZE);
	}
}

void Draw2::SetColor(const unsigned int u_color, const float* const color) {
	glUniform4fv(u_color, 1, color);
}

void Draw2::SetUniform1f(const unsigned int uniform, const float value) {
	glUniform1f(uniform, value);
}

void Draw2::SetUniform2fv(const unsigned int uniform, const float* color) {
	glUniform2fv(uniform, 1, color);
}

void Draw2::SetUniform3fv(const unsigned int uniform, const float* color) {
	glUniform3fv(uniform, 1, color);
}

void Draw2::SetUniform4fv(const unsigned int uniform, const float* color) {
	glUniform4fv(uniform, 1, color);
}

void Draw2::Draw(Mesh& shape) {
	if (!shape.hasVBO()) {
		if (!shape.initVBO()) return;
	}

	glBindVertexArray(shape._VAO);
	glDrawElements(GL_TRIANGLES, shape.countIndex(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Draw2::Lines(Mesh& shape) {
	if (!shape.hasVBO()) {
		if (!shape.initVBO()) return;
	}

	glBindVertexArray(shape._VAO);
	glDrawElements(GL_LINE_LOOP, shape.countIndex(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Draw2::Draw(Model& model) {
	glBindTexture(GL_TEXTURE_2D, model.textureId());
	Draw(model.getMesh());
}

void Draw2::drawPoints(const float* vertices, const unsigned int count) {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_POINTS, 0, count);
}

void Draw2::drawLines(const float* vertices, const unsigned int count) {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_LOOP, 0, count);
}
