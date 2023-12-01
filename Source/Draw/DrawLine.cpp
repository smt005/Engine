
#include "GL/glew.h"

#include "Draw.h"
#include "Shader.h"
#include "Camera/Camera.h"

#include "DrawLine.h"
#include "Object/Line.h"

struct {
	unsigned int program = 0;
	GLuint u_matProjectionView = 0;
	GLuint u_matViewModel = 0;
	GLuint a_position = 0;
	GLuint u_matrix = 0;
	GLuint u_color = 0;
} lineShader;

void DrawLine::prepare()
{
	if (lineShader.program == 0) {
		lineShader.program = Shader::getProgram("Shaders/Line.vert", "Shaders/Line.frag");

		if (!lineShader.program) {
			return;
		}

		lineShader.u_matProjectionView = glGetUniformLocation(lineShader.program, "u_matProjectionView");
		lineShader.u_matViewModel = glGetUniformLocation(lineShader.program, "u_matViewModel");

		lineShader.u_matrix = glGetUniformLocation(lineShader.program, "u_matrix");
		lineShader.u_color = glGetUniformLocation(lineShader.program, "u_color");
	}

	glUseProgram(lineShader.program);
	glUniformMatrix4fv(lineShader.u_matProjectionView, 1, GL_FALSE, Camera::GetLink().ProjectViewFloat());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(lineShader.a_position);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void DrawLine::draw(const Line& line, const glm::mat4x4& matrix)
{
	glUniformMatrix4fv(lineShader.u_matViewModel, 1, GL_FALSE, glm::value_ptr(matrix));

	glUniform4fv(lineShader.u_color, 1, line.color.getDataPtr());
	glLineWidth(line.getLineWidth());

	glVertexAttribPointer(lineShader.a_position, 3, GL_FLOAT, GL_FALSE, 0, line.getData());
	glDrawArrays(line.getType(), 0, line.getCount());
}

void DrawLine::draw(const Line& line)
{
	glUniform4fv(lineShader.u_color, 1, line.color.getDataPtr());
	glLineWidth(line.getLineWidth());

	glVertexAttribPointer(lineShader.a_position, 3, GL_FLOAT, GL_FALSE, 0, line.getData());
	glDrawArrays(line.getType(), 0, line.getCount());
}

void DrawLine::draw(const Greed& greed)
{
	glUniformMatrix4fv(lineShader.u_matViewModel, 1, GL_FALSE, greed.getMatrixFloat());

	DrawLine::draw(greed.lineX);
	DrawLine::draw(greed.lineY);
	DrawLine::draw(greed.lineZ);
	DrawLine::draw(greed.heavyLineX);
	DrawLine::draw(greed.heavyLineY);
	DrawLine::draw(greed.heavyLineZ);
}

void DrawLine::Draw(const float* data, unsigned int count, float widthLine, const float* color, unsigned short int type) {
	glUniform4fv(lineShader.u_color, 1, color);
	glLineWidth(widthLine);

	glVertexAttribPointer(lineShader.a_position, 3, GL_FLOAT, GL_FALSE, 0, data);
	glDrawArrays(type, 0, count);
}

void DrawLine::SetIdentityMatrix() {
	static const glm::mat4x4 identityMatrix(1.f);
	glUniformMatrix4fv(lineShader.u_matViewModel, 1, GL_FALSE, glm::value_ptr(identityMatrix));
}

void DrawLine::SetIdentityMatrixByPos(const float* pos) {
	glm::mat4x4 matrix(1.f);
	matrix[3][0] = pos[0];
	matrix[3][1] = pos[1];
	matrix[3][2] = pos[2];

	glUniformMatrix4fv(lineShader.u_matViewModel, 1, GL_FALSE, glm::value_ptr(matrix));
}