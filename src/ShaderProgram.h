#ifndef _SHADERPROGRAM_H
#define _SHADERPROGRAM_H

#include <OpenGL/gltypes.h>

class ShaderProgram {

public:
    ShaderProgram(const char *vertexShaderFile, const char *fragmentShaderFile);
    ~ShaderProgram();

    GLuint getProgram();
    void use();

    void setUniform1f(const char* id, float value);
    void setUniform2f(const char* id, float value1, float value2);

private:
    GLuint vertShaderInt;
    GLuint fragShaderInt;
    GLuint program;
};

#endif
