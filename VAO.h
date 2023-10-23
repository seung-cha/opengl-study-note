#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm.hpp>

class VAO
{
public:
	/// <summary>
	/// Create a new Vertex Array Object.
	/// </summary>
	/// <param name="data">Array defining vertices</param>
	/// <param name="dataSize">Size of the array, simply sizeof(data)</param>
	/// <param name="usage">Usage const. GL_STATIC_DRAW etc</param>
	/// <param name="elementData">Element Buffer Object if exists</param>
	/// <param name="elemSize">size of the element buffer object. Sizeof(elementData)</param>
	VAO(const void* data, GLsizeiptr dataSize, GLenum usage, const void* elementData = NULL, GLsizeiptr elemSize = 0);

	inline void Bind()
	{
		glBindVertexArray(buffer);
	}

	inline void Unbind()
	{
		glBindVertexArray(0);
	}

	/// <summary>
	/// Call glVertexAttribPointer() with given vertex data
	/// </summary>
	/// <param name="index"></param>
	/// <param name="size"></param>
	/// <param name="type"></param>
	/// <param name="normalized"></param>
	/// <param name="stride">must be n * sizeof(type)</param>
	/// <param name="pointer"></param>
	inline void GLVertexAttribPointer(GLuint index, GLint size, GLenum type,
		GLboolean normalized, GLsizei stride, const void* pointer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindVertexArray(buffer);

		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(index);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/// <summary>
	/// Bind a new buffer and call GLVertexAttribPointer for normal
	/// </summary>
	/// <param name="data"></param>
	/// <param name="dataSize"></param>
	/// <param name="usage"></param>
	/// <param name="index"></param>
	/// <param name="size"></param>
	/// <param name="type"></param>
	/// <param name="normalized"></param>
	/// <param name="stride"></param>
	/// <param name="pointer"></param>
	inline void BindNormalBuffer(const void* data, GLsizeiptr dataSize, GLenum usage, GLuint index, GLint size, GLenum type,
		GLboolean normalized, GLsizei stride, const void* pointer)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);
		
		glBindVertexArray(buffer);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(index);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	};

	inline void BindTextureBuffer(const void* data, GLsizeiptr dataSize, GLenum usage, GLuint index, GLint size, GLenum type,
		GLboolean normalized, GLsizei stride, const void* pointer)
	{
		glGenBuffers(1, &textureVBO);
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);

		glBindVertexArray(buffer);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(index);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	};


	inline int GetFaces() { return faces; }
	inline void SetFaces(unsigned int faces) { this->faces = faces; }

	private:
		unsigned int faces = 0;
		GLuint buffer;
		GLuint VBO;
		GLuint EBO;
		GLuint normalVBO;
		GLuint textureVBO;
};

#endif