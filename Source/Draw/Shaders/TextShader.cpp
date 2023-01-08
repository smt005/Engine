
#pragma once

#include "TextShader.h"
#include "GL/glew.h"
#include "../Shader.h"
#include "../Camera/Camera.h"

void TextShader::prepare()
{
	if (program == 0) {
		program = Shader::getProgram("Shaders/Text.vert", "Shaders/Text.frag");

		if (!program) {
			return;
		}

		u_matProjectionView = glGetUniformLocation(program, "u_matProjectionView");
		u_matViewModel = glGetUniformLocation(program, "u_matViewModel");

		a_position = glGetAttribLocation(program, "a_position");
		a_texCoord = glGetAttribLocation(program, "a_texCoord");
		s_baseMap = glGetUniformLocation(program, "s_baseMap");
	}

	glUseProgram(program);

	//...
	float aspect = Engine::Screen::aspect();
	float size = 1.f;
	float zNear = -1.f;
	float zFar = 1.f;
	glm::mat4x4 matProject;

	if (aspect > 1.f) {
		matProject = glm::ortho(-size * aspect, size * aspect, -size, size, zNear, zFar);
	}
	else {
		matProject = glm::ortho(-size, size, -size / aspect, size / aspect, zNear, zFar);
	}

	glm::vec3 pos(0.f, 0.f, 0.f);
	glm::vec3 direct{ 0.f, -1.f, 0.f };
	glm::vec3 up(0.f, 0.f, 1.f);

	glm::mat4x4 matView = glm::lookAt(pos, pos + direct, up);

	glm::mat4x4 matProjectView = matProject * matView;
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, value_ptr(matProjectView));

	//...
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableVertexAttribArray(a_position);
	glEnableVertexAttribArray(a_texCoord);
	//glEnableVertexAttribArray(a_normal);
}
