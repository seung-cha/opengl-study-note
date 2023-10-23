#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include <iostream>

class Texture
{
	public:
		Texture(const char* source, GLint internalFormat, GLenum format);

		inline void TexParam(GLenum param, GLint value)
		{
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, param, value);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		inline GLuint Get()
		{
			return texture;
		}

	private:
		GLuint texture;
};


#endif