#include "Shader.h"


Shader::Shader(const char* vertSourcePath, const char* fragSourcePath)
{

	const char* fragSource = FileIncluder::ImportFile(fragSourcePath);
	const char* vertSource = FileIncluder::ImportFile(vertSourcePath);

	GLuint fragShader, vertShader;

	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	vertShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(fragShader, 1, &fragSource, NULL);
	glShaderSource(vertShader, 1, &vertSource, NULL);


	glCompileShader(fragShader);
	glCompileShader(vertShader);

	program = glCreateProgram();

	glAttachShader(program, fragShader);
	glAttachShader(program, vertShader);

	glLinkProgram(program);

	glDeleteShader(fragShader);
	glDeleteShader(vertShader);

	free((void*)fragSource);
	free((void*)vertSource);


}
