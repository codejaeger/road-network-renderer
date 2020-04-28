#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLuint LoadTexture( const char * filename, int width, int height);
void FreeTexture( GLuint texture );
#endif 
