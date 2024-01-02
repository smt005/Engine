#include "Draw2/Draw2.h"
#include <glad/gl.h>
#include <Screen.h>

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

void Draw2::Viewport() {
	int widthScreen = Engine::Screen::width();
	int heightScreen = Engine::Screen::height();
	glViewport(0, 0, widthScreen, heightScreen);
}

void Draw2::Draw(Shape& shape) {
	if (!shape.hasVBO()) {
		if (!shape.initVBO()) return;
	}

	glBindVertexArray(shape._VAO);
	glDrawElements(GL_TRIANGLES, shape.countIndex(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
