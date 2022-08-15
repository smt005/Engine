
#pragma once

struct LightShader {
	unsigned int program = 0;
	unsigned int u_matProjectionView = 0;
	unsigned int u_matViewModel = 0;
	unsigned int a_position = 0;
	unsigned int a_texCoord = 0;
	unsigned int a_normal = 0;
	unsigned int s_baseMap = 0;
	unsigned int u_color = 0;

	unsigned int u_lightPos = 0;

	void prepare();
	void reset();
};
