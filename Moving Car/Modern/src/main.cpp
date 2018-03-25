#include "common/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
using namespace glm;


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const char* CAR_BODY_VERTEX_SHADER_FILE_PATH = "shaders/car_body_vertex_shader.vs";
const char* WHEEL_VERTEX_SHADER_FILE_PATH = "shaders/wheel_vertex_shader.vs";
const char* FRAGMENT_SHADER_FILE_PATH = "shaders/fragment_shader.fs";
const float PI = 3.14159265;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void CreatePolygon(GLfloat* polygon_vertices_buffer, int n, GLfloat radius, GLfloat center_x, GLfloat center_y, GLfloat r, GLfloat g, GLfloat b);

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
	GLuint car_body_shader_program = LoadShaders(CAR_BODY_VERTEX_SHADER_FILE_PATH, FRAGMENT_SHADER_FILE_PATH);
	GLuint wheel_shader_program = LoadShaders(WHEEL_VERTEX_SHADER_FILE_PATH, FRAGMENT_SHADER_FILE_PATH);

	// Create triangle points and set their colors
	static const GLint num_of_vertices[] = {
		4, 4, 4, 4, 4, 4, 4, 18, 18
	};

	GLfloat car_vertices[9][5 * 18] = {
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

	static const GLfloat wheel_center[2][2] = {
		{-0.365f, -0.25f}, {0.415f, -0.25f}
	};

	CreatePolygon(car_vertices[7], 16, 0.13f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	CreatePolygon(car_vertices[8], 16, 0.13f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);


	// Create VAO
	GLuint vertex_array[9];
	glGenVertexArrays(9, vertex_array);

  // Create VBO
  GLuint vertex_buffer[9];
  glGenBuffers(9, vertex_buffer);

  for (int i = 0; i < 7; i++) {
		// Bind VAO
		glBindVertexArray(vertex_array[i]);

		// Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[i]);

		// Give our vertices to OpenGL
		glBufferData(GL_ARRAY_BUFFER, num_of_vertices[i] * 5 * sizeof(float), car_vertices[i], GL_STATIC_DRAW);

		// Set vertex attribute
		GLint posAttrib = glGetAttribLocation(car_body_shader_program, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
		                       5*sizeof(float), 0);

		GLint colAttrib = glGetAttribLocation(car_body_shader_program, "in_color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		                       5*sizeof(float), (void*)(2*sizeof(float)));

		// unbind VAO
	  glBindVertexArray(0);
  }

	for (int i = 7; i < 9; i++) {
		// Bind VAO
		glBindVertexArray(vertex_array[i]);

		// Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[i]);

		// Give our vertices to OpenGL
		glBufferData(GL_ARRAY_BUFFER, num_of_vertices[i] * 5 * sizeof(float), car_vertices[i], GL_STATIC_DRAW);

		// Set vertex attribute
		GLint posAttrib = glGetAttribLocation(wheel_shader_program, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
													 5*sizeof(float), 0);

		GLint colAttrib = glGetAttribLocation(wheel_shader_program, "in_color");
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
    glUseProgram(car_body_shader_program);

		// load vertex atrributes from VAO
    for (int i = 0; i < 7; i++) {
    	// Bind VAO
	    glBindVertexArray(vertex_array[i]);

			// Draw
			glDrawArrays(GL_TRIANGLE_FAN, 0, num_of_vertices[i]); // 3 indices starting at 0 -> 1 triangle

			// unbind VAO
			glBindVertexArray(0);
    }

		glUseProgram(wheel_shader_program);


		// load vertex atrributes from VAO
		for (int i = 7; i < 9; i++) {
			mat4 trans;
			trans = translate(trans, glm::vec3(wheel_center[i - 7][0], wheel_center[i - 7][1], 0.0f));
			trans = rotate(trans, (float)-glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
			unsigned int transformLoc = glGetUniformLocation(wheel_shader_program, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(trans));

			// Bind VAO
			glBindVertexArray(vertex_array[i]);

			// Draw
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
	glDeleteBuffers(9, vertex_buffer);
	glDeleteVertexArrays(9, vertex_array);
	glDeleteProgram(car_body_shader_program);
	glDeleteProgram(wheel_shader_program);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void CreatePolygon(GLfloat* polygon_vertices_buffer, int n, GLfloat radius, GLfloat center_x, GLfloat center_y, GLfloat r, GLfloat g, GLfloat b) {
		polygon_vertices_buffer[0] = center_x;
		polygon_vertices_buffer[1] = center_y;
		polygon_vertices_buffer[2] = r;
		polygon_vertices_buffer[3] = g;
		polygon_vertices_buffer[4] = b;

		float arg = 0;
		float inc = 360.0 / n;
		for (int i = 5; i < 5 * (n + 2); i+= 5) {
				polygon_vertices_buffer[i] = center_x + radius * cos(arg * PI / 180);
				polygon_vertices_buffer[i + 1] = center_y + radius * sin(arg * PI / 180);
				polygon_vertices_buffer[i + 2] = r;
				polygon_vertices_buffer[i + 3] = g;
				polygon_vertices_buffer[i + 4] = b;
				arg += inc;
		}
}
