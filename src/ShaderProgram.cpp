
#include "ShaderProgram.h"
#include "util.h"
#include <OpenGL/gl.h>
#include <iostream>
#include <ostream>

ShaderProgram::ShaderProgram(const char *vertexShaderFile, const char *fragmentShaderFile)
{
    std::string fragShaderSrc;
    std::string vertShaderSrc;

    if (getFileContent(vertexShaderFile, vertShaderSrc)) {
        std::cerr << "failed to load shader file: " << vertexShaderFile << std::endl;
        exit(EXIT_FAILURE);
    } else {
        std::cerr << "loaded vertex shader from file:\n"
            << vertShaderSrc << std::endl;
    }

    if (getFileContent(fragmentShaderFile, fragShaderSrc)) {
        std::cerr << "failed to load shader file: " << fragmentShaderFile << std::endl;
        exit(EXIT_FAILURE);
    } else {
        std::cerr << "loaded fragment shader from file:\n"
            << fragShaderSrc << std::endl;
    }

    vertShaderInt = glCreateShader(GL_VERTEX_SHADER);
    const char* vertSource = vertShaderSrc.c_str();
    glShaderSource(vertShaderInt, 1, &vertSource, NULL);
    glCompileShader(vertShaderInt);

    GLint response;
    GLsizei len = 1024;
    GLchar infobuffer[len];
    GLsizei infolength;

    glGetShaderiv(vertShaderInt, GL_COMPILE_STATUS, &response);

    if (response == GL_FALSE) {
        std::cerr << "vertex shader " << vertexShaderFile
            << " could not compile" << std::endl;
        glGetShaderInfoLog(vertShaderInt, len, &infolength, infobuffer);
        std::cerr << infobuffer << "\n";
    } else {
        std::cerr << "vertex shader compiled successfully: "
            << vertexShaderFile << std::endl;
    }

    fragShaderInt = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragSource = fragShaderSrc.c_str();
    glShaderSource(fragShaderInt, 1, &fragSource, NULL);
    glCompileShader(fragShaderInt);

    glGetShaderiv(fragShaderInt, GL_COMPILE_STATUS, &response);

    if (response == GL_FALSE) {
        std::cerr << "fragment shader " << fragmentShaderFile
            << " could not compile" << std::endl;
        glGetShaderInfoLog(fragShaderInt, len, &infolength, infobuffer);
        std::cerr << infobuffer << "\n";
    } else {
        std::cerr << "fragment shader compiled successfully: "
            << fragmentShaderFile << std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, vertShaderInt);
    glAttachShader(program, fragShaderInt);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &response);
    if (response == GL_FALSE) {
        glGetProgramInfoLog(program, len, &infolength, infobuffer);
        std::cerr << "shader program linking failed:\n" << infobuffer << std::endl;
        exit(EXIT_FAILURE);
    } else {
        std::cerr << "shader program linked successfully.\n";
    }
}

void ShaderProgram::setUniform2f(const char* id, float value1, float value2)
{
    glUniform2f(glGetUniformLocation(program, id), value1, value2);
}

void ShaderProgram::setUniform1f(const char* id, float value)
{
    glUniform1f(glGetUniformLocation(program, id), value);
}

void ShaderProgram::use()
{
    glUseProgram(program);
    glLinkProgram(program);
}

GLuint ShaderProgram::getProgram()
{
    return program;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(program);
}
