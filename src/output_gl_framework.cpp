#include "road_network/output_gl_framework.hpp"
#include "road_network/Camera/camera.hpp"

#include "road_network/Road-Model/road_collection.hpp"

extern soc::RoadNetwork *rn;
extern float lastX;
extern float lastY;
extern bool firstMouse;
extern float deltaTime;
extern float lastFrame;
extern Camera camera;
extern GLfloat xrot, yrot, zrot;

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

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  // Close the window if the Esc key was pressed.
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    std::cout << "Escaped\n";
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  // press these keys to rotate the model
  else if (key == GLFW_KEY_A)
    yrot -= 1.0;
  else if (key == GLFW_KEY_D)
    yrot += 1.0;
  else if (key == GLFW_KEY_W)
    xrot -= 1.0;
  else if (key == GLFW_KEY_S)
    xrot += 1.0;
  else if (key == GLFW_KEY_Q)
    zrot -= 1.0;
  else if (key == GLFW_KEY_E)
    zrot += 1.0;
  else if (key == GLFW_KEY_I)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  else if (key == GLFW_KEY_O)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  else if (key == GLFW_KEY_LEFT)
    camera.ProcessKeyboard(LEFT, deltaTime);
  else if (key == GLFW_KEY_RIGHT)
    camera.ProcessKeyboard(RIGHT, deltaTime);
  else if (key == GLFW_KEY_DOWN)
      camera.ProcessKeyboard(DOWN, deltaTime);
  else if (key == GLFW_KEY_UP)
    camera.ProcessKeyboard(UP, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
  {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}

} // End namespace soc
