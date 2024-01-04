#include "Shader2.h"
#include <glad/gl.h>
#include <FileManager.h>
#include <Draw/Camera/Camera.h>

Shader2::Ptr Shader2::current;

Shader2::Shader2(const std::string& vertexFileName, const std::string& fragmentFileName) {
	if (Load(vertexFileName, fragmentFileName)) {
		GetLocation();
	}
}

Shader2::~Shader2() {
	glDeleteProgram(_program);
}

void Shader2::Use() {
	glUseProgram(_program);
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, Camera::GetLink().ProjectViewFloat());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
}

bool Shader2::Load(const std::string& vertexFileName, const std::string& fragmentFileName) {
	static std::filesystem::path shaderFolder = "Shaders";

	std::string vertexShaderSource = Engine::FileManager::readTextFile(shaderFolder / vertexFileName);
	if (vertexShaderSource.empty()) {
		return 0;
	}

	std::string fragmentShaderSource = Engine::FileManager::readTextFile(shaderFolder / fragmentFileName);
	if (fragmentShaderSource.empty()) {
		return 0;
	}

	GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* shaderSource = fragmentShaderSource.c_str();
	glShaderSource(_fragmentShader, 1, &shaderSource, 0);
	glCompileShader(_fragmentShader);

	GLint isShaderCompiled;
	glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);

	if (!isShaderCompiled) {
		int infoLogLength, charactersWritten;
		glGetShaderiv(_fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_fragmentShader, infoLogLength, &charactersWritten, infoLog);

#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader compiled fragment ERROR: %s\n", infoLog);
#endif
		std::cout << "Shader compiled fragment ERROR: " << infoLog << std::endl;

		delete[] infoLog;

		_program = 0;
		return false;
	}

	GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);

	shaderSource = vertexShaderSource.c_str();
	glShaderSource(_vertexShader, 1, &shaderSource, 0);
	glCompileShader(_vertexShader);

	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);

	if (!isShaderCompiled) {
		int infoLogLength, charactersWritten;
		glGetShaderiv(_vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_vertexShader, infoLogLength, &charactersWritten, infoLog);

#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader compiled vertex ERROR: %s\n", infoLog);
#endif
		std::cout << "Shader compiled vertex ERROR: " << infoLog << std::endl;

		delete[] infoLog;

		_program = 0;
		return false;
	}

	_program = glCreateProgram();
	glAttachShader(_program, _fragmentShader);
	glAttachShader(_program, _vertexShader);

	glLinkProgram(_program);

	GLint isLinked;
	glGetProgramiv(_program, GL_LINK_STATUS, &isLinked);

	if (!isLinked) {
		int infoLogLength, charactersWritten;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(_program, infoLogLength, &charactersWritten, infoLog);

#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader linked ERROR: %s \n", infoLog);
#endif
		std::cout << "Shader linked ERROR: " << infoLog << std::endl;

		delete[] infoLog;

		_program = 0;
		return false;
	}

	glDeleteShader(_fragmentShader);
	glDeleteShader(_vertexShader);

	std::cout << "Shader successfully." << std::endl;
	return _program != 0;
}

void Shader2::GetLocation() {
	if (_program == 0) {
		return;
	}

	u_matProjectionView = glGetUniformLocation(_program, "u_matProjectionView");
	u_matViewModel = glGetUniformLocation(_program, "u_matViewModel");
}
