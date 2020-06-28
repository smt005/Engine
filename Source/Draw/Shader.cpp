
#include "GL/glew.h"

#include "Shader.h"
#include "FileManager.h"

unsigned int Shader::getProgram(const std::string& vertexFileName, const std::string& fragmentFileName)
{
	if (glewInit() != GLEW_OK)
	{
		return 0;
	}

    std::string fragmentShaderSource = Engine::FileManager::readTextFile(fragmentFileName);
	if (fragmentShaderSource.empty()) {
		return 0;
	}

	std::string vertexShaderSource = Engine::FileManager::readTextFile(vertexFileName);
	if (vertexShaderSource.empty()) {
		return 0;
	}

	GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* shaderSource = fragmentShaderSource.c_str();
    glShaderSource(_fragmentShader, 1, &shaderSource, 0);
	glCompileShader(_fragmentShader);

	GLint isShaderCompiled;
	glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);

	if (!isShaderCompiled)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetShaderiv(_fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_fragmentShader, infoLogLength, &charactersWritten, infoLog);
        
        #ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader compiled fragment ERROR: %s\n", infoLog);
        #endif
        
		delete[] infoLog;
		return false;
	}

	GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);

    shaderSource = vertexShaderSource.c_str();
    glShaderSource(_vertexShader, 1, &shaderSource, 0);
	glCompileShader(_vertexShader);

	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);
	if (!isShaderCompiled)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetShaderiv(_vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_vertexShader, infoLogLength, &charactersWritten, infoLog);

        #ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader compiled vertex ERROR: %s\n", infoLog);
        #endif

		delete[] infoLog;
		return false;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, _fragmentShader);
	glAttachShader(program, _vertexShader);

	glLinkProgram(program);

	GLint isLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (!isLinked)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, &charactersWritten, infoLog);

        #ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader linked ERROR: %s \n", infoLog);
        #endif

		delete[] infoLog;
		return false;
	}
	
	return program;
}
