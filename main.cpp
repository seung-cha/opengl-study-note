#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileIncluder.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"






void windowFramebufferChangeCallback(GLFWwindow*, int, int);

using namespace std;

int main()
{
	/*
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	*/
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


//---------------------------------------Init---------------------------------------------------------------
	// Initialise GLFW 
	glfwInit();

	// Set up GLFW, feed in version info
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create and bind window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hallo Ward!", NULL, NULL);
	glfwMakeContextCurrent(window);

	//Set window resize callback
	glfwSetFramebufferSizeCallback(window, windowFramebufferChangeCallback);

	//Initialise GLAD
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//Set viewport
	glViewport(0, 0, 800, 600);

//----------------------------------------------------------------------------------------------------------
//------------------------------------Shaders---------------------------------------------------------------
	//Import and create shaders
	const char* vertexShaderSource = Helper::ImportFile("vertexShader/vertexShader.glsl");
	const char* fragmentShaderSource = Helper::ImportFile("fragmentShader/fragmentShader.glsl");

	GLuint vertexShaderID, fragmentShaderID;

	//Create shader IDs, source and then compile them
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	//Create a central shader program and bind the shaders.
	GLuint shaderProgramID;
	shaderProgramID = glCreateProgram();

	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);

	//From this point, the shaders can be free'd.
	glLinkProgram(shaderProgramID);
	
	//Free the shaders, including the source
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	free((void*)vertexShaderSource);
	free((void*)fragmentShaderSource);

//----------------------------------------------------------------------------------------------------------
//------------------------------------Buffers---------------------------------------------------------------

	//Create VAO
	GLuint vertexArrayBufferID;
	glGenVertexArrays(1, &vertexArrayBufferID);
	//Bind VAO, indicating any other buffers will be associated to this.
	glBindVertexArray(vertexArrayBufferID);


	//Create VBO
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);

	//Bind & Generate data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Assign buffer usage & open location
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	//Create EBO
	GLuint elementBufferID;
	glGenBuffers(1, &elementBufferID);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//Unbind VBO/VAO/EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	



//----------------------------------------------------------------------------------------------------------
//----------------------------------Rendering---------------------------------------------------------------

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.13f, 0.13f, 0.13f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		//Enable shader & vertex array
		glUseProgram(shaderProgramID);
		glBindVertexArray(vertexArrayBufferID);

		//Draw using VBO
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//Draw using EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//Unbind shader & vertex array
		glUseProgram(0);
		glBindVertexArray(0);


		//Process miscellaneous things
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	



	glfwTerminate();



}


void windowFramebufferChangeCallback(GLFWwindow* window, int width, int height)
{
	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);

}