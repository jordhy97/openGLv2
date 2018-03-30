#ifndef SHADER_H
#define SHADER_H

class Shader {
public:
  // Constructor, read and build the shader
  Shader(const char vertex_shader_path[], const char fragment_shader_path[]);

  // Destructor
  ~Shader();

  // Use the shader
  void use();

  // Utility uniform functions
  void setBool(const char name[], bool value) const;
  void setInt(const char name[], int value) const;
  void setFloat(const char name[], float value) const;
  void setMat4(const char name[], float* value) const;

  // Getter
  unsigned int getID() const;
  int getAttribLocation(const char name[]) const;

private:
  // The program ID
  unsigned int id_;
};

#endif
