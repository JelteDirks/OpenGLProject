#ifndef _UTIL_H
#define _UTIL_H

#include "GLFW/glfw3.h"
#include <string>

#define STRINGIFY(x) #x // makes a string of x
#define TOSTRING(x) STRINGIFY(x) // expands x

int getFileContent(const std::string& filename, std::string& content);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif
