#include "ShaderInterface.h"
#include <glad/gl.h>
#include <iostream>
#include <Log.h>
#include <FileManager.h>

ShaderInterface::~ShaderInterface() {
	glDeleteProgram(_program);
}

void ShaderInterface::Use() {
	glUseProgram(_program);
}

void ShaderInterface::GetLocation() {
	if (_program == 0) {
		return;
	}
}

bool ShaderInterface::Init(const std::string& vertexFileName, const std::string& fragmentFileName) {
	if (Load(vertexFileName, fragmentFileName)) {
		GetLocation();
		return true;
	}
	return false;
}

bool ShaderInterface::Load(const std::string& vertexFileName, const std::string& fragmentFileName) {
	static std::filesystem::path shaderFolder = "Shaders";

	std::string vertexShaderSource = Engine::FileManager::readTextFile(shaderFolder / vertexFileName);
	if (vertexShaderSource.empty()) {
		return 0;
	}

	std::string fragmentShaderSource = Engine::FileManager::readTextFile(shaderFolder / fragmentFileName);
	if (fragmentShaderSource.empty()) {
		return 0;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* shaderSource = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &shaderSource, 0);
	glCompileShader(fragmentShader);

	GLint isShaderCompiled;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);

	if (!isShaderCompiled) {
		int infoLogLength, charactersWritten;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(fragmentShader, infoLogLength, &charactersWritten, infoLog);

		Log("Shader compiled fragment ERROR: {}", infoLog);
		delete[] infoLog;

		_program = 0;
		return false;
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	shaderSource = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &shaderSource, 0);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);

	if (!isShaderCompiled) {
		int infoLogLength, charactersWritten;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(vertexShader, infoLogLength, &charactersWritten, infoLog);

		Log("Shader compiled vertex ERROR: {}", infoLog);
		delete[] infoLog;

		_program = 0;
		return false;
	}

	_program = glCreateProgram();
	glAttachShader(_program, fragmentShader);
	glAttachShader(_program, vertexShader);

	glLinkProgram(_program);

	GLint isLinked;
	glGetProgramiv(_program, GL_LINK_STATUS, &isLinked);

	if (!isLinked) {
		int infoLogLength, charactersWritten;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(_program, infoLogLength, &charactersWritten, infoLog);

		Log("Shader linked ERROR: {}", infoLog);
		delete[] infoLog;

		_program = 0;
		return false;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	Log("Init shader successfully. {}, {}", vertexFileName, fragmentFileName);
	return _program != 0;
}
