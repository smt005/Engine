
#include "glad/gl.h"
#include "Shaders/LightShader.h"

#include "DrawLight.h"
#include "Screen.h"
#include "Shader.h"
#include "Object/Mesh.h"
#include "Object/Shape.h"
#include "Object/Model.h"
#include "Object/Object.h"
#include "Object/Triangle.h"
#include "Object/Map.h"
#include "Object/Texture.h"

#include "DrawLine.h"
#include "Object/Line.h"

#include <iostream>
#include <Draw2/Shader2.h>

float DrawLight::_clearColor[4] = { 0.3f, 0.6f , 0.9f , 1.0f };
float DrawLight::_lightPos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
unsigned int DrawLight::curentBufer = 0;
unsigned int DrawLight::currentTexture = 0;

LightShader shader;

void DrawLight::setClearColor(const float r, const float g, const float b, const float a)
{
	_clearColor[0] = r;
	_clearColor[1] = g;
	_clearColor[2] = b;
	_clearColor[3] = a;

	glClearColor(r, g, b, a);
}

const float * const DrawLight::getClearColor()
{
	return _clearColor;
}

void DrawLight::setLightPos(const float x, const float y, const float z) {
	_lightPos[0] = x;
	_lightPos[1] = y;
	_lightPos[2] = z;
	_lightPos[3] = 1.0;
}

void DrawLight::clearColor()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawLight::clearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void DrawLight::viewport()
{
	int widthScreen = Engine::Screen::width();
	int heightScreen = Engine::Screen::height();
	glViewport(0, 0, widthScreen, heightScreen);
}

void DrawLight::prepare()
{
	shader.prepare();

	curentBufer = 0;
	currentTexture = 0;
}

void DrawLight::resetShader() {
	shader.reset();
	//texture.reset();
	curentBufer = 0;
	currentTexture = 0;
}

void DrawLight::draw(Mesh& mesh)
{
	if (!mesh.hasVBO()) {
		if (!mesh.initVBO()) return;
	}

	if (curentBufer != mesh.bufferIndexes())
	{
		curentBufer = mesh.bufferIndexes();

		glBindBuffer(GL_ARRAY_BUFFER, mesh.bufferVertexes());
		glVertexAttribPointer(shader.a_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.bufferTexCoords());
		glVertexAttribPointer(shader.a_texCoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.bufferNormals());
		glVertexAttribPointer(shader.a_normal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.bufferIndexes());
	}
	glDrawElements(GL_TRIANGLES, mesh.countIndex(), GL_UNSIGNED_INT, 0);
}

void DrawLight::draw(Model& model)
{
	unsigned int textureId = model.textureId();
	if (currentTexture != textureId)
	{
		currentTexture = textureId;
		
		glUniform1i(shader.s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, currentTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glUniform4fv(shader.u_color, 1, model.getDataPtr());

	glUniform4fv(shader.u_lightPos, 1, _lightPos);

	if (model.getShape().Loaded()) {
		Mesh& mesh = model.getMesh();
		draw(mesh);
	}
}

void DrawLight::draw(Object& object)
{	
	const glm::mat4x4& matrix = object.getMatrix();
	glUniformMatrix4fv(shader.u_matViewModel, 1, GL_FALSE, glm::value_ptr(matrix));

	Model& model = object.getModel();
	draw(model);
}

void DrawLight::draw(Map& map)
{
	for (auto& object : map.GetObjects()) {
		if (object->visible()) {
			draw(*object);
		}
	}
}

void DrawLight::draw(const Triangle& triangle)
{
	glUniformMatrix4fv(shader.u_matViewModel, 1, GL_FALSE, triangle.getMatrixFloat());

	const glm::mat4x4& mat = triangle.getMatrix();
	const float* matrix = triangle.getMatrixFloat();
	glUniformMatrix4fv(shader.u_matViewModel, 1, GL_FALSE, matrix);

	unsigned int textureId = triangle.textureId();
	if (currentTexture != textureId)
	{
		currentTexture = textureId;

		glUniform1i(shader.s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, currentTexture);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (!triangle.hasVBO()) {
		if (!triangle.initVBO()) return;
	}
	glUniform4fv(shader.u_color, 1, triangle.getDataPtr());

	if (curentBufer != triangle.bufferVertexes())
	{
		curentBufer = triangle.bufferVertexes();

		glBindBuffer(GL_ARRAY_BUFFER, triangle.bufferVertexes());
		glEnableVertexAttribArray(shader.a_position);
		glVertexAttribPointer(shader.a_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, triangle.bufferTexCoords());
		glEnableVertexAttribArray(shader.a_texCoord);
		glVertexAttribPointer(shader.a_texCoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	}

	glDrawArrays(GL_TRIANGLES, 0, triangle.countVertex());
}

void DrawLight::SetMatrux(const glm::mat4x4& matrix) {
	glUniformMatrix4fv(shader.u_matViewModel, 1, GL_FALSE, glm::value_ptr(matrix));
}
