#include "glad/glad.h"
#include "shader.hpp"
#include <iostream>
#include <fstream>
#include <utility>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(std::string source, GLenum shaderType)
    : id{ glCreateShader(shaderType) }, type{ shaderType }
{
    const char* csource = source.c_str();
    glShaderSource(id, 1, &csource, NULL);
    glCompileShader(id);
    GLint status = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
	std::cout << "Shader Compilation failed. ID: " << id << std::endl;
	log();
    }
}

Shader::~Shader()
{
    glDeleteShader(id);
}

void Shader::log()
{
    if (glIsShader(id))
    {
	int logLength = 0;
	int maxLength = 0;

	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

	char* log = new char[maxLength];

	glGetShaderInfoLog(id, maxLength, &logLength, log);
	if (logLength > 0)
	{
	    std::cout << log << std::endl;
	}

	delete[] log;
    }
    else
    {
	std::cout << "The shader " << id << " is not a shader, but was"
	    "requested to be logged as one" << std::endl;
    }
}

std::pair<std::string, std::string> Shader::source2shader(std::string path)
{
    std::string sources[2];
    std::string vs;
    std::string fs;
    std::stringstream buffer;

    std::ifstream source(path);
    if (!source.is_open())
    {
	std:: cout << "Unable to read shader source." << std::endl;
	return std::make_pair("error","error");
    }
    else
    {
	buffer << source.rdbuf();
	size_t pos = buffer.str().find("arandomstring");
	if (pos == std::string::npos)
	{
	    std::cout << "Invalid shader file. Recall the vertex shader"
		      << "should be first, then the string \"arandomstr"
		      << "ing\", then the fragment shader"
		      << std::endl;;
	    return std::make_pair("ERROR","CHECK LOGS");
	}
	else
	{
	    vs = sources[0] = buffer.str().substr(0, pos);
	    // 14 == size_in_chars("arandomstring\n")
	    fs = sources[1] = buffer.str().substr(pos + 14);


	    return std::make_pair(vs,fs);
	}
    }
}

ShaderProgram::ShaderProgram(Shader vertex, Shader frag)
    : id{ glCreateProgram() }
{
    glAttachShader(id, vertex.id);
    glAttachShader(id, frag.id);

    glLinkProgram(id);
    GLint status = GL_TRUE;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
	std::cout << "Shader program linking failed. ID: " << id << std::endl;
	log();
    }
}

void ShaderProgram::log()
{
    if (glIsProgram(id))
    {
	int logLength = 0;
	int maxLength = 0;

	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

	char* log = new char[maxLength];

	glGetProgramInfoLog(id, maxLength, &logLength, log);
	if (logLength > 0)
	{
	    std::cout << log << std::endl;
	}

	delete[] log;
    }
    else
    {
	std::cout << "The program << id << is not a program, but was requested"
		  << " to be logged as one" << std::endl;
    }
}

void ShaderProgram::use()
{
    glUseProgram(id);
    return;
}

void ShaderProgram::set(std::string variableName, glm::vec3 vector)
{
    GLint location = glGetUniformLocation(id, variableName.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vector));
}
