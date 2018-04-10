#ifndef MESH_H
#define MESH_H

#include "shader.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
using namespace std;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_coordinate;
};

struct Texture {
  unsigned int id;
  string type;
  string path;
};

class Mesh {
public:
  // Constructor
  Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures, const glm::vec3& diffuse_color);

  // Render the mesh
  void render(Shader& shader) const;

private:
  // Initializes all the buffer objects/arrays
  void setupMesh();

  // Mesh data
  vector<Vertex> vertices_;
  vector<unsigned int> indices_;
  vector<Texture> textures_;
  glm::vec3 diffuse_color_;
  unsigned int VAO_;

  // Render data
  unsigned int VBO_, EBO_;

};

#endif
