
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

void DrawLight::Test() {
	static bool _init_ = false;
	static GLuint VBO, VAO, EBO;
	static Shader2 shader;

	if (!_init_) {
		shader.Load("Default.vert", "Default.frag");

		// Set up vertex data (and buffer(s)) and attribute pointers
		//GLfloat vertices[] = {
		//  // First triangle
		//   0.5f,  0.5f,  // Top Right
		//   0.5f, -0.5f,  // Bottom Right
		//  -0.5f,  0.5f,  // Top Left
		//  // Second triangle
		//   0.5f, -0.5f,  // Bottom Right
		//  -0.5f, -0.5f,  // Bottom Left
		//  -0.5f,  0.5f   // Top Left
		//};
		GLfloat vertices[] = {
			 0.5f,  0.5f, 0.0f,  // Top Right
			 0.5f, -0.5f, 0.0f,  // Bottom Right
			-0.5f, -0.5f, 0.0f,  // Bottom Left
			-0.5f,  0.5f, 0.0f   // Top Left
		};
		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3,  // First Triangle
			1, 2, 3   // Second Triangle
		};
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

		_init_ = true;
	}

	//...
	{
		// Render
		// Clear the colorbuffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle
		shader.Bind();
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
