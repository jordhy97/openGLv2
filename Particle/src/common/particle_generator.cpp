#include "particle_generator.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <algorithm>

// Constructor
ParticleGenerator::ParticleGenerator(const Shader& shader, const char texture_path[], const string &texture_directory, unsigned int amount)
    : shader_(shader) {
    texture_.id = textureFromFile(texture_path, texture_directory);
    amount_ = amount;
    last_used_particle_ = 0;
    init();
}

// Update all particles
void ParticleGenerator::update(float dt, unsigned int new_particles, glm::vec3 camera_position, glm::vec3 offset) {
  // Add new particles
  for (unsigned int i = 0; i < new_particles; i++) {
    int unused_particle = firstUnusedParticle();
    respawnParticle(particles_[unused_particle], offset);
  }

  // Update all particles
  particles_count_ = 0;
  for (unsigned int i = 0; i < amount_; i++) {
    Particle &p = particles_[i];
    if (p.life > 0.0f) {
      p.life -= dt; // reduce life
      p.color.a -= (dt * 100);
      if (p.life > 0.0f) {	// particle is alive, thus update
        // p.position -= p.velocity * dt;
        // p.color.a -= dt * 2.5;


        // Simulate simple physics : gravity only, no collisions
        // p.velocity += glm::vec3(0.0f,-9.81f, 0.0f) * dt * 0.5f;
        p.position += p.velocity * dt;

        // wind effect
        p.position.x += -(rand() % 3)/8.0f * dt;
        p.position.y += -(rand() % 5 - 2)/8.0f * dt;
        p.position.z += -(rand() % 7 - 2)/8.0f * dt;

        p.cameradistance = glm::length2(p.position - camera_position);

        //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

        // Fill the GPU buffer
        g_particle_position_size_data_[4*particles_count_+0] = p.position.x;
        g_particle_position_size_data_[4*particles_count_+1] = p.position.y;
        g_particle_position_size_data_[4*particles_count_+2] = p.position.z;

        g_particle_position_size_data_[4*particles_count_+3] = p.size;

        g_particle_color_data_[4*particles_count_+0] = p.color.r;
        g_particle_color_data_[4*particles_count_+1] = p.color.g;
        g_particle_color_data_[4*particles_count_+2] = p.color.b;
        g_particle_color_data_[4*particles_count_+3] = p.color.a;
      } else {
        // Particles that just died will be put at the end of the buffer in SortParticles();
  			p.cameradistance = -1.0f;
      }
      particles_count_++;
    }
  }

  sortParticles();
}

// Render all particles
void ParticleGenerator::render() {
  // Use additive blending to give it a 'glow' effect
  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer_);
  // glBufferData(GL_ARRAY_BUFFER, amount_ * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count_ * sizeof(GLfloat) * 4, &g_particle_position_size_data_[0]);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer_);
  // glBufferData(GL_ARRAY_BUFFER, amount_ * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count_ * sizeof(GLubyte) * 4, &g_particle_color_data_[0]);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_.id);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles_count_);
  glBindVertexArray(0);

  // Don't forget to reset to default blending mode
}

void ParticleGenerator::sortParticles() {
  std::sort(particles_.begin(), particles_.end());
}

// Initializes buffer and vertex attributes
void ParticleGenerator::init() {
  // The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};
  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);

  GLuint billboard_vertex_buffer_;
	glGenBuffers(1, &billboard_vertex_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer_);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, amount_ * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer_);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, amount_ * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

  // 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer_);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer_);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer_);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

  glBindVertexArray(0);

  // Create this->amount default particle instances
  for (unsigned int i = 0; i < amount_; i++) {
    particles_.push_back(Particle());
  }

  g_particle_position_size_data_.resize(4 * amount_);
  g_particle_color_data_.resize(4 * amount_);
}

// Returns the first Particle index that's currently unused e.g. life <= 0.0f or 0 if no particle is currently inactive
unsigned int ParticleGenerator::firstUnusedParticle() {
  // First search from last used particle, this will usually return almost instantly
  for (unsigned int i = last_used_particle_; i < amount_; i++) {
    if (particles_[i].life <= 0.0f) {
      last_used_particle_ = i;
      return last_used_particle_;
    }
  }

  // Otherwise, do a linear search
  for (unsigned int i = 0; i < last_used_particle_; i++) {
    if (particles_[i].life <= 0.0f) {
      last_used_particle_ = i;
      return last_used_particle_;
    }
  }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    last_used_particle_ = 0;
    return last_used_particle_;
}

// Respawns particle
void ParticleGenerator::respawnParticle(Particle &particle, glm::vec3 offset) {
  // GLfloat random = ((rand() % 11) - 10) / 10000.0f;
  // particle.position = random + offset;
  // particle.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  // //  particle.Life = 1.0f;
  // //  particle.Velocity = object.Velocity * 0.1f;
  // // particle.position = glm::vec3(0.0f, 0.0f, 0.0f);
  // // particle.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  // particle.velocity.y = (rand() % 100001 - 50000) / 100000.0f;
  // particle.velocity.x = rand() % 100001 / 100000.0f;
  // particle.velocity.z = (rand() % 100001 - 50000) / 100000.0f;
  //
  // particle.life = 1.0f;

  particle.life = 1.0f; // This particle will live 5 seconds.
	particle.position = glm::vec3(-0.244f, -0.065f, 2.48f);

	float spread = 1.5f;
	glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
	// Very bad way to generate a random direction;
	// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
	// combined with some user-controlled parameters (main direction, spread, etc)
	glm::vec3 randomdir = glm::vec3(
		(rand()%2000 - 1000.0f)/1000.0f,
		(rand()%2000 - 1000.0f)/1000.0f,
		(rand()%2000 - 1000.0f)/1000.0f
	);

	particle.velocity = glm::vec3(0.0f, 0.02f, 0.0f);


	// Very bad way to generate a random color
  int color = 200 + rand() % 56;
	particle.color.r = color;
	particle.color.g = color;
	particle.color.b = color;
	particle.color.a = 255;
	particle.size =  0.025f;
}
