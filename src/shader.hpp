#ifndef SHADER_H
#define SHADER_H
#include "glad/glad.h"
#include <string>
#include <utility>
#include <glm/glm.hpp>

class Shader
{
public:
    GLuint id;
    GLenum type;

    Shader(std::string source, GLenum shaderType);
    ~Shader();
    void log();

    static std::pair<std::string, std::string> source2shader(std::string path);
};

class ShaderProgram
{
public:
    GLuint id;

    ShaderProgram(Shader vertex, Shader frag);
    void log();
    void use();

    void set(std::string variableName, glm::vec3 vector);
};
#endif
