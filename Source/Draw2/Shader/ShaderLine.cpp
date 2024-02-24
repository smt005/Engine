#include "ShaderLine.h"
#include <glad/gl.h>
#include <FileManager.h>
#include <Draw/Camera/Camera.h>

ShaderLine::Ptr ShaderLine::current;
unsigned int ShaderLine::u_matProjectionView = 0;
unsigned int ShaderLine::u_matViewModel = 0;
unsigned int ShaderLine::u_color = 0;

void ShaderLine::Use() {
	glUseProgram(_program);
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, Camera::GetLink().ProjectViewFloat());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
}

void ShaderLine::GetLocation() {
	if (_program == 0) {
		return;
	}

	u_matProjectionView = glGetUniformLocation(_program, "u_matProjectionView");
	u_matViewModel = glGetUniformLocation(_program, "u_matViewModel");

	u_color = glGetUniformLocation(_program, "u_color");
}
