#ifndef SHADER_H

#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "FileIncluder.h"

#include <glm.hpp>

class Shader
{
	public:
		Shader(const char* vertSourcePath, const char* fragSourcePath);

		inline void Use()
		{
			glUseProgram(program);
		}

		inline void Unuse()
		{
			glActiveTexture(GL_TEXTURE0);
			glUseProgram(0);
		}

		inline GLuint GetLoc(const char* loc)
		{
			return glGetUniformLocation(program, loc);
		}

		inline void SetTexture(int loc, GLuint texture)
		{
			Use();
			glActiveTexture(GL_TEXTURE0 + loc);
			glBindTexture(GL_TEXTURE_2D, texture);
		}


		inline void SetPerspective(glm::mat4& view, glm::mat4& proj)
		{
			Use();
			glUniformMatrix4fv(GetLoc("view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(GetLoc("perspective"), 1, GL_FALSE, &proj[0][0]);
		}

		inline void SetModel(glm::mat4& transform, const char* name = "model")
		{
			Use();
			glUniformMatrix4fv(GetLoc(name), 1, GL_FALSE, &transform[0][0]);
		}

		inline GLuint GetProg()
		{
			return program;
		}

	private:
		GLuint program;
		


};


#endif