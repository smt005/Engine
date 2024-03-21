#include "ShaderDefault.h"
#include <glad/gl.h>
#include <FileManager.h>
#include <Draw/Camera/Camera.h>

unsigned int ShaderDefault::u_matProjectionView = 0;
unsigned int ShaderDefault::u_matViewModel = 0;
unsigned int ShaderDefault::u_color = 0;

void ShaderDefault::Use() {
	glUseProgram(_program);
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, Camera::GetLink().ProjectViewFloat());
}

void ShaderDefault::GetLocation() {
	if (_program == 0) {
		return;
	}

	u_matProjectionView = glGetUniformLocation(_program, "u_matProjectionView");
	u_matViewModel = glGetUniformLocation(_program, "u_matViewModel");
	u_color = glGetUniformLocation(_program, "u_color");
}
