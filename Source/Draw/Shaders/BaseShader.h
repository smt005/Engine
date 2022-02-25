
#pragma once

#include "GL/glew.h"
#include "../Shader.h"
#include "../Camera.h"

struct BaseShader {
	unsigned int program = 0;
	GLuint u_matProjectionView = 0;
	GLuint u_matViewModel = 0;
	GLuint a_position = 0;
	GLuint a_texCoord = 0;
	GLuint s_baseMap = 0;
	GLuint u_color = 0;

	virtual void prepare()
	{
		if (program == 0) {
			program = Shader::getProgram("Shaders/Base.vert", "Shaders/Base.frag");

			if (!program) {
				return;
			}

			u_matProjectionView = glGetUniformLocation(program, "u_matProjectionView");
			u_matViewModel = glGetUniformLocation(program, "u_matViewModel");

			a_position = glGetAttribLocation(program, "a_position");
			a_texCoord = glGetAttribLocation(program, "a_texCoord");

			s_baseMap = glGetUniformLocation(program, "s_baseMap");
			u_color = glGetUniformLocation(program, "u_color");
		}

		glUseProgram(program);
		glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, Camera::current.matPV());

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnableVertexAttribArray(a_position);
		glEnableVertexAttribArray(a_texCoord);
	}
};
