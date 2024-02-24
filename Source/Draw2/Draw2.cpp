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

void Draw2::Draw(Mesh& shape) {
	if (!shape.hasVBO()) {
		if (!shape.initVBO()) return;
	}

	glBindVertexArray(shape._VAO);
	glDrawElements(GL_TRIANGLES, shape.countIndex(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Draw2::Draw(Model& model) {
	// TODO:
	unsigned int textureId = model.textureId();
	glBindTexture(GL_TEXTURE_2D, textureId);

	Draw(model.getMesh()); // TODO:
}


void Draw2::drawLine() {
	float color[] = { 0.3f, 0.6f, 0.9f, 1.0f };
	float widthLine = 5.f;

	glUniform4fv(ShaderLine::u_color, 1, color);
	glLineWidth(widthLine);

	GLfloat vertices[] = { 0.f, 99.5f, 0.f,
							-99.75f, -99.5f, 0.f,
							0.5f, -99.75f, 0.f };

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), vertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
}