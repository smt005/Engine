
#include "UIScreen.h"

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Draw/Shader.h"
#include "Screen.h"

namespace UI {
	Sample _sample;

	void Screen::init() {
		//_samples.emplace_back();

		//_sample.setScale(glm::vec3(0.1f));
		_sample.setPos(glm::vec3(0.0f, 0.0f, 0.0f));

		{
			Sample* sampleChild = new Sample();
			sampleChild->setScale(glm::vec3(0.1f));
			//sampleChild->setPos(glm::vec3(1.0f, 1.0f, -0.01f));
			_sample._childs.emplace_back(sampleChild);
		}

		{
			Sample* sampleChild = new Sample();
			sampleChild->setScale(glm::vec3(0.1f));
			sampleChild->setPos(glm::vec3(1.0f, 1.0f, -0.01f));
			_sample._childs.emplace_back(sampleChild);
		}
	}

	struct {
		unsigned int program = 0;
		GLuint u_matProjectionView = 0;
		GLuint u_matViewModel = 0;
		GLuint a_position = 0;
		GLuint a_texCoord = 0;
		GLuint s_baseMap = 0;
		GLuint u_color = 0;
	} baseShader;

	float _clearColor[4] = { 0.3f, 0.6f , 0.9f , 1.0f };
	unsigned int program = 0;
	TexturePtr texture;
	unsigned int curentBufer = 0;

	unsigned int currentTexture = 0;

	void Screen::prepare()
	{
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

		glm::mat4x4 mat(1.0f);
		//mat = glm::translate(mat, glm::vec3(-1.0f, -1.0f, 0.0f));

		float widthScreen = static_cast<float>(Engine::Screen::width());
		float heightScreen = static_cast<float>(Engine::Screen::height());

		glm::vec3 scale(1.0f);
		if (heightScreen < widthScreen && widthScreen != 0) {
			scale.x = heightScreen / widthScreen;
		}
		else if (heightScreen != 0.0f) {
			scale.y = widthScreen / heightScreen;
		}
		mat = glm::scale(mat, scale);

		glUniformMatrix4fv(baseShader.u_matProjectionView, 1, GL_FALSE, glm::value_ptr(mat));

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnableVertexAttribArray(baseShader.a_position);
		glEnableVertexAttribArray(baseShader.a_texCoord);

		curentBufer = 0;
		currentTexture = 0;
	}

	void Screen::draw() {
		draw(_sample);
	}

	void Screen::draw(Sample& sample) {
		//glm::mat4x4 mat(1.0f);
		//mat = glm::scale(mat, glm::vec3(2.0f, 2.0f, 1.0f));
		//glUniformMatrix4fv(baseShader.u_matViewModel, 1, GL_FALSE, glm::value_ptr(mat));

		glUniformMatrix4fv(baseShader.u_matViewModel, 1, GL_FALSE, sample.getMatrixFloat());

		unsigned int textureId = _sample._texture ?_sample._texture->id() : 0;
		if (currentTexture != textureId)
		{
			currentTexture = textureId;

			glUniform1i(baseShader.s_baseMap, 0);
			glBindTexture(GL_TEXTURE_2D, currentTexture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		if (!_sample.hasVBO()) {
			if (!_sample.initVBO()) return;
		}

		float color[4] = { 1.0f, 1.0f, 1.0f, 0.25f };
		glUniform4fv(baseShader.u_color, 1, color);

		glBindBuffer(GL_ARRAY_BUFFER, _sample._buffer[0]);
		glEnableVertexAttribArray(baseShader.a_position);
		glVertexAttribPointer(baseShader.a_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, _sample._buffer[1]);
		glEnableVertexAttribArray(baseShader.a_texCoord);
		glVertexAttribPointer(baseShader.a_texCoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glDrawArrays(_sample._type, 0, _sample._count);

		for (auto& item : sample._childs) {
			draw(*item);
		}
	}
}
