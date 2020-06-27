
// GLEW #include "GL/glew.h"

#include "Draw.h"
#include "Camera.h"
#include "Window.h"
#include "Shader.h"
#include "Object/Mesh.h"
#include "Object/Shape.h"
#include "Object/Model.h"
#include "Object/Object.h"
#include "Object/Glider.h"
#include "Object/Triangle.h"
#include "Object/Map.h"
#include "Object/Texture.h"

float _clearColor[4] = { 0.3f, 0.6f , 0.9f , 1.0f };
unsigned int program = 0;
TexturePtr texture;
unsigned int curentBufer = 0;
unsigned int currentTexture = 0;

/* GLEW
struct {
	unsigned int program = 0;
	GLuint u_matProjectionView = 0;
	GLuint u_matViewModel = 0;
	GLuint a_position = 0;
	GLuint a_texCoord = 0;
	GLuint s_baseMap = 0;
	GLuint u_color = 0;
} baseShader;
GLEW */

void Draw::setClearColor(const float r, const float g, const float b, const float a)
{
	_clearColor[0] = r;
	_clearColor[1] = g;
	_clearColor[2] = b;
	_clearColor[3] = a;

	// GLEW glClearColor(r, g, b, a);
}

const float * const Draw::getClearColor()
{
	return _clearColor;
}

void Draw::clearColor()
{
	// GLEW glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw::viewport()
{
	int widthScreen = Engine::Window::width();
	int heightScreen = Engine::Window::height();
	// GLEW glViewport(0, 0, widthScreen, heightScreen);
}

void Draw::prepare()
{
	/* GLEW 
	if (baseShader.program == 0) {
		baseShader.program = Shader::getProgram("Shaders/Base.vert", "Shaders/Base.frag");
	
		if (!baseShader.program) {
			return;
		}

		baseShader.u_matProjectionView = glGetUniformLocation(baseShader.program, "u_matProjectionView");
		baseShader.u_matViewModel = glGetUniformLocation(baseShader.program, "u_matViewModel");

		baseShader.a_position = glGetAttribLocation(baseShader.program, "a_position");
		baseShader.a_texCoord = glGetAttribLocation(baseShader.program, "a_texCoord");

		baseShader.s_baseMap = glGetUniformLocation(baseShader.program, "s_baseMap");
		baseShader.u_color = glGetUniformLocation(baseShader.program, "u_color");
	}

	glUseProgram(baseShader.program);
	glUniformMatrix4fv(baseShader.u_matProjectionView, 1, GL_FALSE, Camera::current.matPV());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableVertexAttribArray(baseShader.a_position);
	glEnableVertexAttribArray(baseShader.a_texCoord);

	curentBufer = 0;
	currentTexture = 0;
	GLEW */
}

void Draw::draw(Mesh& mesh)
{
	if (!mesh.hasVBO()) {
		if (!mesh.initVBO()) return;
	}

	if (curentBufer != mesh.bufferIndexes())
	{
		curentBufer = mesh.bufferIndexes();

		/* GLEW
		glBindBuffer(GL_ARRAY_BUFFER, mesh.bufferVertexes());
		glVertexAttribPointer(baseShader.a_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.bufferTexCoords());
		glVertexAttribPointer(baseShader.a_texCoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.bufferIndexes());
		GLEW */
	}

	// GLEW glDrawElements(GL_TRIANGLES, mesh.countIndex(), GL_UNSIGNED_SHORT, 0);
}

void Draw::draw(Model& model)
{
	unsigned int textureId = model.textureId();
	if (currentTexture != textureId)
	{
		currentTexture = textureId;
		
		// GLEW glUniform1i(baseShader.s_baseMap, 0);
		// GLEW glBindTexture(GL_TEXTURE_2D, currentTexture);
	}

	// GLEW glUniform4fv(baseShader.u_color, 1, model.getDataPtr());

	Mesh& mesh = model.getMesh();
	draw(mesh);
}

void Draw::draw(Object& object)
{	
	const glm::mat4x4& matrix = object.getMatrix();
	// GLEW glUniformMatrix4fv(baseShader.u_matViewModel, 1, GL_FALSE, glm::value_ptr(matrix));

	Model& model = object.getModel();
	draw(model);
}

void Draw::draw(Map& map)
{
	for (auto& object : map.objects) {
		if (object->visible()) {
			draw(*object);
		}
	}

	for (auto& glider : map.gliders) {
		if (glider->visible()) {
			draw(*glider);
		}
	}
}

void Draw::draw(const Triangle& triangle)
{
	//glUniformMatrix4fv(baseShader.u_matViewModel, 1, GL_FALSE, triangle.getMatrixFloat());

	const glm::mat4x4& mat = triangle.getMatrix();
	const float* matrix = triangle.getMatrixFloat();
	// GLEW glUniformMatrix4fv(baseShader.u_matViewModel, 1, GL_FALSE, matrix);

	unsigned int textureId = triangle.textureId();
	if (currentTexture != textureId)
	{
		currentTexture = textureId;

		// GLEW glUniform1i(baseShader.s_baseMap, 0);
		// GLEW glBindTexture(GL_TEXTURE_2D, currentTexture);

		// GLEW glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// GLEW glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	if (!triangle.hasVBO()) {
		if (!triangle.initVBO()) return;
	}

	// GLEW glUniform4fv(baseShader.u_color, 1, triangle.getDataPtr());

	if (curentBufer != triangle.bufferVertexes())
	{
		curentBufer = triangle.bufferVertexes();

		// GLEW glBindBuffer(GL_ARRAY_BUFFER, triangle.bufferVertexes());
		// GLEW glEnableVertexAttribArray(baseShader.a_position);
		// GLEW glVertexAttribPointer(baseShader.a_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		// GLEW glBindBuffer(GL_ARRAY_BUFFER, triangle.bufferTexCoords());
		// GLEW glEnableVertexAttribArray(baseShader.a_texCoord);
		// GLEW glVertexAttribPointer(baseShader.a_texCoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	}

	// GLEW glDrawArrays(GL_TRIANGLES, 0, triangle.countVertex());
}