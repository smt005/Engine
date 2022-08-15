
#pragma once

#include "LightShader.h"
#include "GL/glew.h"
#include "../Shader.h"
#include "../Camera.h"

void LightShader::prepare()
{
	if (program == 0) {
		program = Shader::getProgram("Shaders/Light.vert", "Shaders/Light.frag");

		if (!program) {
			return;
		}

		u_matProjectionView = glGetUniformLocation(program, "u_matProjectionView");
		u_matViewModel = glGetUniformLocation(program, "u_matViewModel");

		a_position = glGetAttribLocation(program, "a_position");
		a_texCoord = glGetAttribLocation(program, "a_texCoord");
		a_normal = glGetAttribLocation(program, "a_normal");

		s_baseMap = glGetUniformLocation(program, "s_baseMap");
		u_color = glGetUniformLocation(program, "u_color");

		u_lightPos = glGetUniformLocation(program, "u_lightPos");
	}

	glUseProgram(program);
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, Camera::current.matPV());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableVertexAttribArray(a_position);
	glEnableVertexAttribArray(a_texCoord);
	glEnableVertexAttribArray(a_normal);
}

void LightShader::reset() {
	glDeleteProgram(program);

	program = 0;
	u_matProjectionView = 0;
	u_matViewModel = 0;
	a_position = 0;
	a_texCoord = 0;
	a_normal = 0;
	s_baseMap = 0;
	u_color = 0;
	u_lightPos = 0;
}
