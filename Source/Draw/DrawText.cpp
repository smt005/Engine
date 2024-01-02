
#include "DrawText.h"
#include "Object/Text.h"
#include "Object/Map.h"
#include "Object/Object.h"
#include "Object/Shape.h"
#include "Object/Model.h"
#include "Screen.h"
#include "Shader.h"
#include "Shaders/TextShader.h"
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include "Draw/DrawLight.h"

TextShader textShader;
Mesh textMesh;

unsigned int DrawText::curentBufer;
unsigned int DrawText::currentTexture;

void DrawText::ClearColor()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void DrawText::Viewport()
{
	int widthScreen = Engine::Screen::width();
	int heightScreen = Engine::Screen::height();
	glViewport(0, 0, widthScreen, heightScreen);
}

void DrawText::Prepare()
{
	textShader.prepare();

	curentBufer = 0;
	currentTexture = 0;
}

void DrawText::Draw(Engine::Text& text) {
	DrawText::Prepare();

	glm::mat4x4 matrix(1.f);
	float fText = (float)text.Width() / (float)text.Height();

	matrix = glm::translate(matrix, glm::vec3(text._offsetX * fText, text._offsetY, text._offsetZ));

	float scale = (float)text.Height() / (float)Engine::Screen::width();

	matrix = glm::scale(matrix, glm::vec3(scale * fText, 1.f, scale));

	glUniformMatrix4fv(textShader.u_matViewModel, 1, GL_FALSE, glm::value_ptr(matrix));

	unsigned int textureId = text.IdTexture();
	if (currentTexture != textureId)
	{
		currentTexture = textureId;

		glUniform1i(textShader.s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, currentTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (!textMesh.hasVBO()) {
		Mesh::MakeRectangle(textMesh);
		if (!textMesh.initVBO()) return;
	}

	if (curentBufer != textMesh.bufferIndexes()) {
		curentBufer = textMesh.bufferIndexes();

		glBindBuffer(GL_ARRAY_BUFFER, textMesh.bufferVertexes());
		glVertexAttribPointer(textShader.a_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, textMesh.bufferTexCoords());
		glVertexAttribPointer(textShader.a_texCoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textMesh.bufferIndexes());
	}
	glDrawElements(GL_TRIANGLES, textMesh.countIndex(), GL_UNSIGNED_INT, 0);
}
