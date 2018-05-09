#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glm/glm.hpp>

#include "shader.h"
#include "mesh.h"

// Represents a single particle and its state
struct Particle {
  glm::vec3 position, velocity;
  glm::vec4 color;
  float size;
  float life; // Remaining life of the particle. if < 0 : dead and unused.

  Particle() : position(0.0f), velocity(0.0f), color(1.0f), size(0.0f), life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator {
public:
  // Constructor
  ParticleGenerator(const Shader& shader, const char texture_path[], const string &texture_directory, unsigned int amount);

  // Update all particles
  void update(float dt, unsigned int new_particles, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

  // Render all particles
  void render();

private:
  // Initializes buffer and vertex attributes
  void init();

  // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
  unsigned int firstUnusedParticle();

  // Respawns particle
  void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

  // State
  std::vector<Particle> particles_;
  unsigned int amount_;

  // Render state
  Shader shader_;
  Texture texture_;
  unsigned int VAO_;

  // Stores the index of the last particle used (for quick access to next dead particle)
  unsigned int last_used_particle_;


};

#endif
