#include "ShaderLine.h"
#include <glad/gl.h>
#include <FileManager.h>
#include <Draw/Camera/Camera.h>

unsigned int ShaderLine::u_color = 0;

unsigned int ShaderLineP::u_matProjectionView = 0;
unsigned int ShaderLineP::u_color = 0;

unsigned int ShaderLinePM::u_matProjectionView = 0;
unsigned int ShaderLinePM::u_matViewModel = 0;
unsigned int ShaderLinePM::u_color = 0;

unsigned int ShaderGravityPoint::u_matProjectionView = 0;
unsigned int ShaderGravityPoint::u_color = 0;
unsigned int ShaderGravityPoint::u_factor = 0;
unsigned int ShaderGravityPoint::u_range = 0;
unsigned int ShaderGravityPoint::u_rangeZ = 0;
unsigned int ShaderGravityPoint::u_body_position = 0;
unsigned int ShaderGravityPoint::u_body_color = 0;

//...
void ShaderLine::Use() {
	glUseProgram(_program);

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

	u_color = glGetUniformLocation(_program, "u_color");
}

//...
void ShaderLineP::Use() {
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

void ShaderLineP::GetLocation() {
	if (_program == 0) {
		return;
	}

	u_matProjectionView = glGetUniformLocation(_program, "u_matProjectionView");
	u_color = glGetUniformLocation(_program, "u_color");
}

//...
void ShaderLinePM::Use() {
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

void ShaderLinePM::GetLocation() {
	if (_program == 0) {
		return;
	}

	u_matProjectionView = glGetUniformLocation(_program, "u_matProjectionView");
	u_matViewModel = glGetUniformLocation(_program, "u_matViewModel");

	u_color = glGetUniformLocation(_program, "u_color");
}

// ShaderGravityPoint
void ShaderGravityPoint::Use() {
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

void ShaderGravityPoint::GetLocation() {
	if (_program == 0) {
		return;
	}

	u_matProjectionView = glGetUniformLocation(_program, "u_matProjectionView");
	u_color = glGetUniformLocation(_program, "u_color");
	u_factor = glGetUniformLocation(_program, "u_factor");
	u_range = glGetUniformLocation(_program, "u_range");
	u_rangeZ = glGetUniformLocation(_program, "u_rangeZ");

	u_body_position = glGetUniformLocation(_program, "u_body_position");
	u_body_color = glGetUniformLocation(_program, "u_body_color");
}
