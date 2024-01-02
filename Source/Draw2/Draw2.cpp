#include "Draw2/Draw2.h"
#include <glad/gl.h>
#include <Screen.h>

float Draw2::clearColor[4] = { 0.333f, 0.666f , 0.999f , 1.0f };

void Draw2::SetClearColor(float r, float g, float b, float a) {
	clearColor[0] = r;
	clearColor[1] = g;
	clearColor[2] = b;
	clearColor[3] = a;

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

template<typename ModelT>
static void Draw2::Model(ModelT&& object) {
}


template<typename ObjectT>
static void Draw2::Object(ObjectT&& object) {
}

template<typename ObjectsT>
static void Draw2::Objects(ObjectsT&& object) {
}
