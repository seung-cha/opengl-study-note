#include "VAO.h"

#include <iostream>

VAO::VAO(const void* data, GLsizeiptr dataSize, GLenum usage, const void* elementData, GLsizeiptr elemSize)
{
	glGenVertexArrays(1, &buffer);
	glBindVertexArray(buffer);

	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);

	if (elementData != NULL)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemSize, elementData, usage);
	}


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
