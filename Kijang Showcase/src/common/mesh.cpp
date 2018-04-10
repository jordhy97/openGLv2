#include "mesh.h"
#include <GL/glew.h>
#include <iostream>
// Constructor
Mesh::Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures, const glm::vec3& diffuse_color) {
  vertices_ = vertices;
  indices_ = indices;
  textures_ = textures;
  diffuse_color_ = diffuse_color;

  // Set the vertex buffers and its attribute pointers
  setupMesh();
}

// Render the mesh
void Mesh::render(Shader& shader) const {
  // bind appropriate textures
  unsigned int diffuse_number  = 1;
  unsigned int specular_number = 1;

  for (unsigned int i = 0; i < textures_.size(); i++) {
    // active proper texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);

    // retrieve texture number (the N in diffuse_textureN)
    string number;
    string name = textures_[i].type;
    if (name == "texture_diffuse") {
      number = to_string(diffuse_number++);
    } else if (name == "texture_specular") {
      number = to_string(specular_number++);
    }

  	// set the sampler to the correct texture unit
    shader.setInt((name + number).c_str(), i);

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, textures_[i].id);
  }

  shader.setVec3("diffuse_color", diffuse_color_);
  // render mesh
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}

// Initializes all the buffer objects/arrays
void Mesh::setupMesh() {
  // create buffers/arrays
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  glBindVertexArray(VAO_);

  // load data into vertex buffers
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

  // set the vertex attribute pointers
  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  // vertex texture coordinate
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinate));

  glBindVertexArray(0);
}
