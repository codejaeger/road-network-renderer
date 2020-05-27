#ifndef texture_hpp
#define texture_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>
#include <fstream>

GLuint LoadTexture(const char *filename, int width, int height);
void FreeTexture(GLuint texture);

#endif /* texture_hpp */
