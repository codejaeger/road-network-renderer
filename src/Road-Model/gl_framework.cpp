#include "road_network/Road-Model/road_collection.hpp"

extern GLfloat c_xrot, c_yrot, c_zrot;
extern soc::RoadNetwork *rn;
extern bool enable_perspective;
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
    // initialize the road network from the file
    // which contains the points along the roads
    std::cout << "Enter the name of the raw file to draw the road-network"
              << std::endl;
    std::string s;
    std::cin >> s;
    rn->initRoadNetwork("./models/Bezier-Model/" + s);
  }

  else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    enable_perspective = !enable_perspective;
  // press these keys to rotate the model
  else if (key == GLFW_KEY_A)
    c_yrot -= 1.0;
  else if (key == GLFW_KEY_D)
    c_yrot += 1.0;
  else if (key == GLFW_KEY_W)
    c_xrot -= 1.0;
  else if (key == GLFW_KEY_S)
    c_xrot += 1.0;
  else if (key == GLFW_KEY_Q)
    c_zrot -= 1.0;
  else if (key == GLFW_KEY_E)
    c_zrot += 1.0;
}

// GLFW mouse button callback
void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {}

} // End namespace soc
