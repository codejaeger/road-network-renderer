#include "road_network/Multiple-Roads/gl_framework.hpp"
#include "road_network/Multiple-Roads/path.hpp"

extern soc::Paths *p;

namespace soc {
// Initialize GL State
void initGL(void) {
  // Set framebuffer clear color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  // Set depth buffer furthest depth
  glClearDepth(1.0);
  // Set depth test to less-than
  glDepthFunc(GL_LESS);
  // Enable depth testing
  glEnable(GL_DEPTH_TEST);
}

// GLFW Error Callback
void error_callback(int error, const char *description) {
  std::cerr << description << std::endl;
}

// GLFW framebuffer resize callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // Resize the viewport to fit the window size - draw to entire window
  glViewport(0, 0, width, height);
}

// GLFW keyboard callback
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  // Close the window if the Esc key was pressed.
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    std::cout << "Escaped\n";
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    p->resume();
  }
  else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
    p->next();
  }
  else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
    p->previous();
  }
  else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    p->delete_last();
  }
  // Saves the control points in a raw file if the S key was pressed.
  // else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
  //   p->save();
  // }
  // // Loads the control points from a raw file if the L key was pressed.
  // else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
  //   p->load();
  // }
  // Resumes the input of control points if the R key was pressed.

}

// GLFW mouse button callback
void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  // Takes the location of left click as the input of control points.
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (p->return_input_status())
      p->getPoints(window);
  }
  // Stops the input of control points if the right mouse button was pressed.
  else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    p->stop();
  }
}

} // End namespace soc
