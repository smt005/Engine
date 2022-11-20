
#include "GL/glew.h"
#include "Shaders/BaseShader.h"

#include "Draw.h"
#include "Screen.h"
#include "Shader.h"
#include "Object/Mesh.h"
#include "Object/Shape.h"
#include "Object/Model.h"
#include "Object/Object.h"
#include "Object/Triangle.h"
#include "Object/Map.h"
#include "Object/Texture.h"

float Draw::_clearColor[4] = { 0.3f, 0.6f , 0.9f , 1.0f };
unsigned int Draw::curentBufer = 0;
unsigned int Draw::currentTexture = 0;

BaseShader shader;


void Draw::setClearColor(const float r, const float g, const float b, const float a)
{
	_clearColor[0] = r;
	_clearColor[1] = g;
	_clearColor[2] = b;
	_clearColor[3] = a;

	glClearColor(r, g, b, a);
}

const float * const Draw::getClearColor()
{
	return _clearColor;
}

void Draw::clearColor()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw::viewport()
{
	int widthScreen = Engine::Screen::width();
	int heightScreen = Engine::Screen::height();
	glViewport(0, 0, widthScreen, heightScreen);
}

void Draw::prepare()
{
	shader.prepare();

	curentBufer = 0;
	currentTexture = 0;
}

void Draw::draw(Mesh& mesh)
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.bufferIndexes());
	}
	glDrawElements(GL_TRIANGLES, mesh.countIndex(), GL_UNSIGNED_INT, 0);
}

void Draw::draw(Model& model)
{
	unsigned int textureId = model.textureId();
	if (currentTexture != textureId)
	{
		currentTexture = textureId;
		
		glUniform1i(shader.s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, currentTexture);
	}

	glUniform4fv(shader.u_color, 1, model.getDataPtr());

	Mesh& mesh = model.getMesh();
	draw(mesh);
}

void Draw::draw(Object& object)
{	
	const glm::mat4x4& matrix = object.getMatrix();
	glUniformMatrix4fv(shader.u_matViewModel, 1, GL_FALSE, glm::value_ptr(matrix));

	Model& model = object.getModel();
	draw(model);
}

void Draw::draw(Map& map)
{
	for (auto& object : map.GetObjects()) {
		if (object->visible()) {
			draw(*object);
		}
	}
}

void Draw::draw(const Triangle& triangle)
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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