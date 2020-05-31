#ifndef output_gl_framework_hpp
#define output_gl_framework_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace soc {

// Initialize GL State
void initGL(void);
// GLFW Error Callback
void error_callback(int error, const char *description);
// GLFW framebuffer resize callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// GLFW keyboard callback
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);
// GLFW CursorPosCallback
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// GLFW ScrollCallback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
} // End namespace soc

#endif /* output_gl_framework_hpp */
