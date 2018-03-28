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
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Moving Car", NULL, NULL);
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

	// Init car body
	static const GLfloat car_body_vertices[7][20] = {
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
	GLuint car_body_vertex_array;
	glGenVertexArrays(1, &car_body_vertex_array);

  // Create VBO
  GLuint car_body_vertex_buffer;
  glGenBuffers(1, &car_body_vertex_buffer);

  // Bind VAO
	glBindVertexArray(car_body_vertex_array);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, car_body_vertex_buffer);

	// Give our vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(car_body_vertices), car_body_vertices, GL_STATIC_DRAW);

	// Set vertex attribute
	GLint posAttrib = glGetAttribLocation(car_body_shader_program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
	                      5 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(car_body_shader_program, "in_color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		                    5*  sizeof(float), (void*)(2 * sizeof(float)));

	// unbind VAO
	glBindVertexArray(0);

	// Init wheel
	static const GLfloat wheel_center[2][2] = {
		{-0.365f, -0.25f}, {0.415f, -0.25f}
	};

	GLfloat wheel_vertices[5000];
	static const int wheel_vertices_count[] = {
		362, 46, 46, 46, 46, 46, 46, 362
	};
	int num_segments = 360;
	int idx = 0;
	wheel_vertices[idx++] = 0.0f;
	wheel_vertices[idx++] = 0.0f;
	wheel_vertices[idx++] = 0.0f;
	wheel_vertices[idx++] = 0.0f;
	wheel_vertices[idx++] = 0.0f;

	for (int i = 0; i <= num_segments; i++) {
		float theta = 2.0f * PI * i / float(num_segments);
		wheel_vertices[idx++] = 0.13f * cos(theta);
		wheel_vertices[idx++] = 0.13f * sin(theta);
		wheel_vertices[idx++] = 0.0f;
		wheel_vertices[idx++] = 0.0f;
		wheel_vertices[idx++] = 0.0f;
	}

	for (int i = 1; i <= 6; i++) {
		wheel_vertices[idx++] = 0.0f;
		wheel_vertices[idx++] = 0.0f;
		wheel_vertices[idx++] = 0.75f;
		wheel_vertices[idx++] = 0.75f;
		wheel_vertices[idx++] = 0.75f;
		for (int j = (i - 1) * 60 + 8; j <= (i - 1) * 60 + 52; j++) {
			float theta = 2.0f * PI * j / float(num_segments);
			wheel_vertices[idx++] = 0.09f * cos(theta);
			wheel_vertices[idx++] = 0.09f * sin(theta);
			wheel_vertices[idx++] = 0.75f;
			wheel_vertices[idx++] = 0.75f;
			wheel_vertices[idx++] = 0.75f;
		}
	}

	wheel_vertices[idx++] = 0.0f;
	wheel_vertices[idx++] = 0.0f;
	wheel_vertices[idx++] = 0.0f;
	wheel_vertices[idx++] = 0.0f;
	wheel_vertices[idx++] = 0.0f;

	for (int i = 0; i <= num_segments; i++) {
		float theta = 2.0f * PI * i / float(num_segments);
		wheel_vertices[idx++] = 0.025f * cos(theta);
		wheel_vertices[idx++] = 0.025f * sin(theta);
		wheel_vertices[idx++] = 0.0f;
		wheel_vertices[idx++] = 0.0f;
		wheel_vertices[idx++] = 0.0f;
	}

	// Create VAO
	GLuint wheel_vertex_array;
	glGenVertexArrays(1, &wheel_vertex_array);

	// Create VBO
	GLuint wheel_vertex_buffer;
	glGenBuffers(1, &wheel_vertex_buffer);

	// Bind VAO
	glBindVertexArray(wheel_vertex_array);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, wheel_vertex_buffer);

	// Give our vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(wheel_vertices), wheel_vertices, GL_STATIC_DRAW);

	// Set vertex attribute
	posAttrib = glGetAttribLocation(wheel_shader_program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
												5 * sizeof(float), 0);

	colAttrib = glGetAttribLocation(wheel_shader_program, "in_color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
												5 * sizeof(float), (void*)(2 * sizeof(float)));

	// unbind VAO
	glBindVertexArray(0);

	// Main loop
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw car body
		glUseProgram(car_body_shader_program);
		glBindVertexArray(car_body_vertex_array);
		int offset = 0;
    for (int i = 0; i < 7; i++) {
			int vertices_count = sizeof(car_body_vertices[i]) / (5 * sizeof(float));
			glDrawArrays(GL_TRIANGLE_FAN, offset, vertices_count);
			offset += vertices_count;
    }
		glBindVertexArray(0);

		// Draw wheels
		glUseProgram(wheel_shader_program);
		glBindVertexArray(wheel_vertex_array);
		for (int i = 0; i < 2; i++) {
			mat4 trans;
			trans = translate(trans, vec3(wheel_center[i][0], wheel_center[i][1], 0.0f));
			trans = rotate(trans, (float)-glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
			unsigned int transformLoc = glGetUniformLocation(wheel_shader_program, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(trans));
			offset = 0;
			for (int i = 0; i < 8; i++) {
				glDrawArrays(GL_TRIANGLE_FAN, offset, wheel_vertices_count[i]);
				offset += wheel_vertices_count[i];
			}
		}
		glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Check if the ESC key was pressed or the window was closed
	} while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO, VAO, and shader program
	glDeleteBuffers(1, &car_body_vertex_buffer);
	glDeleteVertexArrays(1, &car_body_vertex_array);
	glDeleteBuffers(1, &wheel_vertex_buffer);
	glDeleteVertexArrays(1, &wheel_vertex_array);
	glDeleteProgram(car_body_shader_program);
	glDeleteProgram(wheel_shader_program);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
