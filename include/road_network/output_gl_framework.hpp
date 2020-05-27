#ifndef output_gl_framework_hpp
#define output_gl_framework_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "road_network/Road-Model/road_collection.hpp"
#include "road_network/Traffic-Manager/manager.hpp"

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset))

#define PI_MATH 3.14159265

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

#endif /* output_gl_framework_hpp */
