#include "road_network/Multiple-Roads/simulation.hpp"
#include "road_network/Multiple-Roads/path.hpp"

soc::Paths *p;

void initBuffersGL(void) { p = new soc::Paths(); }

void renderGL(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p->renderLine();
}
void deleteBuffersGL(){
  delete p;
}

int main(int argc, char **argv) {
  //! The pointer to the GLFW window
  GLFWwindow *window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(soc::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  // We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // We don't want the old OpenGL
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(WIDTH, HEIGHT, "Simulation", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  //! Make the window's context current
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  // Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    // Problem: glewInit failed, something is seriously wrong.
    std::cerr << "GLEW Init Failed : %s" << std::endl;
  }

  // Print and see what context got enabled
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;

  // Keyboard Callback
  glfwSetKeyCallback(window, soc::key_callback);
  // Mouse Button Callback
  glfwSetMouseButtonCallback(window, soc::mouse_button_callback);
  // Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, soc::framebuffer_size_callback);
  // To ensure we can capture key being pressed
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  // To ensure we can capture mouse button being pressed
  glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

  // Initialize GL state
  soc::initGL();
  initBuffersGL();
  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0) {
    // Render here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGL(window);
    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }
  deleteBuffersGL();
  glfwTerminate();

  return 0;
}
