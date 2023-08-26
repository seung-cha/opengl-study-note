#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}";



void windowFramebufferChangeCallback(GLFWwindow*, int, int);


int main()
{

	//Initialise glfw
	glfwInit();

	//Set glfw version to 3.3 as well as core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0,
		0.5f, -0.5f, 0,
		0.0f, 0.5f, 0,
	};


	//Create a new glfw window which does nothing on its own.
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hi", NULL, NULL);

	if (!window)
	{
		std::cout << "Something went wrong!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Make all glfw calls direct to this window
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, windowFramebufferChangeCallback);


	
	//Initialse glad to use GL functions this must be done after a window is made current.
	gladLoadGL();

	//Set up rendering viewport
	glViewport(0, 0, 800, 600);

	//Create a vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// GLUint source, GLSizei count, GLchar** string,  GLint* length
	// Register 'counter' number of registers to 'source' vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	//Compile the shader
	glCompileShader(vertexShader);


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	//Create a shader program that takes vertex shader and fragment shader.
	GLuint shaderProgram = glCreateProgram();

	//Attach vertex/fragement shader to this object
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//Link the two. From now on, shader sources can be deleted.
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	
	//Vertex buffer to contain vertex data.
	GLuint VBO;

	//Vertex array to govern the usage of vertex data.
	GLuint VAO;


	//Create a vertex array and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//Create a vertex buffer and bind it
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Bind data to the array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Specify the data and location
	//This modifies the currently-bound vertex buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//Enable the array, location specified by the argument
	glEnableVertexAttribArray(0);

	//Unbind the two buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//Write color to back buffer then apply it

	//Swap the front buffer to the back buffer

	while (!glfwWindowShouldClose(window))
	{

		//Wipe out the back buffer with specified colour
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Apply the colour
		glClear(GL_COLOR_BUFFER_BIT);

		//Use specified shader program to render image
		glUseProgram(shaderProgram);
		//Bind a vertex array to receive data from
		glBindVertexArray(VAO);

		//Draw
		glDrawArrays(GL_TRIANGLES, 0, 3);



		//Swap the double buffers
		glfwSwapBuffers(window);
		//Process inputs etc
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}


void windowFramebufferChangeCallback(GLFWwindow* window, int width, int height)
{
	//glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);

}