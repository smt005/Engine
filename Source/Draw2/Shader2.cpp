#include "Shader2.h"
#include <glad/gl.h>
#include <FileManager.h>

Shader2::Shader2(const std::string& vertexFileName, const std::string& fragmentFileName) {
	Load(vertexFileName, fragmentFileName);
}

Shader2::~Shader2() {
	// TODO:
}

void Shader2::Bind() {
	glUseProgram(_program);
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
#ifdef _DEBUG
		int infoLogLength, charactersWritten;
		glGetShaderiv(_fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_fragmentShader, infoLogLength, &charactersWritten, infoLog);

		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader compiled fragment ERROR: %s\n", infoLog);

		delete[] infoLog;
#endif
		_program = 0;
		return false;
	}

	GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);

	shaderSource = vertexShaderSource.c_str();
	glShaderSource(_vertexShader, 1, &shaderSource, 0);
	glCompileShader(_vertexShader);

	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);

	if (!isShaderCompiled) {
#ifdef _DEBUG
		int infoLogLength, charactersWritten;
		glGetShaderiv(_vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_vertexShader, infoLogLength, &charactersWritten, infoLog);

		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader compiled vertex ERROR: %s\n", infoLog);

		delete[] infoLog;
#endif
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
#ifdef _DEBUG
		int infoLogLength, charactersWritten;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(_program, infoLogLength, &charactersWritten, infoLog);

		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader linked ERROR: %s \n", infoLog);

		delete[] infoLog;
#endif
		_program = 0;
		return false;
	}

	return _program != 0;
}
