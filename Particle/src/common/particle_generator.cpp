#include "particle_generator.h"
#include <GL/glew.h>
#include <iostream>

// Constructor
ParticleGenerator::ParticleGenerator(const Shader& shader, const char texture_path[], const string &texture_directory, unsigned int amount)
    : shader_(shader) {
    texture_.id = textureFromFile(texture_path, texture_directory);
    amount_ = amount;
    last_used_particle_ = 0;
    init();
}

// Update all particles
void ParticleGenerator::update(float dt, unsigned int new_particles, glm::vec3 offset) {
  // Add new particles
  for (unsigned int i = 0; i < new_particles; i++) {
    int unused_particle = firstUnusedParticle();
    respawnParticle(particles_[unused_particle], offset);
  }

  // Update all particles
  for (unsigned int i = 0; i < amount_; ++i) {
    Particle &p = particles_[i];
    p.life -= dt; // reduce life
    if (p.life > 0.0f) {	// particle is alive, thus update
      p.position -= p.velocity * dt;
      p.color.a -= dt * 2.5;
    }
  }
}

// Render all particles
void ParticleGenerator::render() {
  // Use additive blending to give it a 'glow' effect
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  for (Particle particle : particles_) {
    if (particle.life > 0.0f) {
      shader_.setVec3("offset", particle.position);
      shader_.setVec4("color", particle.color);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture_.id);
      glBindVertexArray(VAO_);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
  // Don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Initializes buffer and vertex attributes
void ParticleGenerator::init() {
  // Set up mesh and attribute properties
  unsigned int VBO;
  float particle_quad[] = {
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
  };
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO_);

  // Fill mesh buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

  // Set mesh attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
  glBindVertexArray(0);

  // Create this->amount default particle instances
  for (unsigned int i = 0; i < amount_; i++) {
    particles_.push_back(Particle());
  }
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
  GLfloat random = ((rand() % 11) - 10) / 100.0f;
  particle.position = random + offset;
  particle.color = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
  //  particle.Life = 1.0f;
  //  particle.Velocity = object.Velocity * 0.1f;
  // particle.position = glm::vec2(0.0f, 0.0f);
  // particle.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  particle.velocity.y = rand() % 101 / 100.0f;
  // particle.velocity.x = (rand() % 11 - 10) / 100.0f;
  // particle.velocity.z = (rand() % 11 - 10) / 100.0f;

  particle.life = 1.0f;
}
