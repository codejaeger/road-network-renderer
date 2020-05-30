#ifndef input_gl_framework_hpp
#define input_gl_framework_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset))

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
// GLFW mouse button callback
void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods);
} // End namespace soc

#endif /* input_gl_framework_hpp */
