#include "common/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define VERTEX_SHADER_FILE_PATH  "shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE_PATH "shaders/fragment_shader.glsl"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

int main() {
	// Initialize GLFW
	if(!glfwInit()) {
    cerr << "Failed to initialize GLFW" << endl;
		return -1;
	}

	// Configure GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set openGL version (4.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		cerr << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Set viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// Create and compile GLSL program from the shaders
	GLuint shader_program = LoadShaders(VERTEX_SHADER_FILE_PATH, FRAGMENT_SHADER_FILE_PATH);

	// Create triangle points and set their colors
	static const GLint num_of_vertices[] = {
		4, 4, 4, 4, 4, 4, 4
	};

	static const GLfloat car[7][25] = {
		// exhaust
		{
			-0.7f, -0.1f, 0.566f, 0.566f, 0.566f,
			-0.7f, -0.2f, 0.566f, 0.566f, 0.566f,
			-0.6f, -0.2f, 0.566f, 0.566f, 0.566f,
			-0.6f, -0.1f, 0.566f, 0.566f, 0.566f
		},
		
		// body
		{
			-0.6f, 0.15f, 1.0f, 0.0f, 0.0f,
			-0.6f, -0.25f, 1.0f, 0.0f, 0.0f,
			0.75f, -0.25f, 1.0f, 0.0f, 0.0f,
			0.75f, 0.15f, 1.0f, 0.0f, 0.0f
		},

		//upper body
		{
			-0.45f, 0.15f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.15f, 1.0f, 0.0f, 0.0f,
			0.35f, 0.4f, 1.0f, 0.0f, 0.0f,
			-0.3f, 0.4f, 1.0f, 0.0f, 0.0f
		},

		// back window
		{
			-0.365f, 0.15f, 0.8f, 0.8f, 0.8f,
			0.0f, 0.15f, 0.8f, 0.8f, 0.8f,
			0.0f, 0.35f, 0.8f, 0.8f, 0.8f,
			-0.25f, 0.35f, 0.8f, 0.8f, 0.8f
		},

		// front window
		{
			0.075f, 0.15f, 0.8f, 0.8f, 0.8f,
			0.415f, 0.15f, 0.8f, 0.8f, 0.8f,
			0.3f, 0.35f, 0.8f, 0.8f, 0.8f,
			0.075f, 0.35f, 0.8f, 0.8f, 0.8f
		},

		// back door
		{
			-0.365f, 0.15f, 0.85f, 0.0f, 0.0f,
			-0.365f, -0.1f, 0.85f, 0.0f, 0.0f,
			0.0f, -0.1f, 0.85f, 0.0f, 0.0f,
			0.0f, 0.15f, 0.85f, 0.0f, 0.0f
		},

		// front door
		{
			0.075f, 0.15f, 0.85f, 0.0f, 0.0f,
			0.075f, -0.1f, 0.85f, 0.0f, 0.0f,
			0.415f, -0.1f, 0.85f, 0.0f, 0.0f,
			0.415f, 0.15f, 0.85f, 0.0f, 0.0f
		}
	};

	// Create VAO
	GLuint vertex_array[7];
	glGenVertexArrays(7, vertex_array);

  // Create VBO
  GLuint vertex_buffer[7];
  glGenBuffers(7, vertex_buffer);

  for (int i = 0; i < 7; i++) {
		// Bind VAO
		glBindVertexArray(vertex_array[i]);

		// Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[i]);

		// Give our vertices to OpenGL
		glBufferData(GL_ARRAY_BUFFER, num_of_vertices[i] * 5 * sizeof(float), car[i], GL_STATIC_DRAW);
  
		// Set vertex attribute
		GLint posAttrib = glGetAttribLocation(shader_program, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
		                       5*sizeof(float), 0);

		GLint colAttrib = glGetAttribLocation(shader_program, "in_color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		                       5*sizeof(float), (void*)(2*sizeof(float)));

		// unbind VAO
	  glBindVertexArray(0);
  }



	// Main loop
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// use shader program
    glUseProgram(shader_program);

		// load vertex atrributes from VAO
    for (int i = 0; i < 7; i++) {
    	// Bind VAO
	    glBindVertexArray(vertex_array[i]);

			// Draw the triangle
			glDrawArrays(GL_TRIANGLE_FAN, 0, num_of_vertices[i]); // 3 indices starting at 0 -> 1 triangle

			// unbind VAO
			glBindVertexArray(0);
    }

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Check if the ESC key was pressed or the window was closed
	} while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO, VAO, and shader program
	glDeleteBuffers(7, vertex_buffer);
	glDeleteVertexArrays(7, vertex_array);
	glDeleteProgram(shader_program);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
