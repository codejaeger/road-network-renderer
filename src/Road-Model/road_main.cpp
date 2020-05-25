#include "road_network/Road-Model/road_main.hpp"

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;

glm::mat4 modelview_matrix;

// pointer to RoadNetwork object
soc::RoadNetwork *rn;
soc::Graph *g;
soc::Manager *m;

void initBuffersGL(std::string file) {
  rn = new soc::RoadNetwork(0.02, 0.05, file); // road-depth=0.02, road-width=0.1
  rn->initRoadNetwork();
  g = rn->getGraph();
  // for (unsigned int i = 0; i < g->v.size(); i++) {
  //   std::cout << g->v[i].origin[0] << "pp" << g->v[i].origin[1] << std::endl;
  // }
  m = new soc::Manager(g, 1, 3);
}

void renderGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  // Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot),
                                  glm::vec3(1.0f, 0.0f, 0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot),
                                  glm::vec3(0.0f, 0.0f, 1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos, c_ypos, c_zpos, 1.0) * c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x, c_up_y, c_up_z, 1.0) * c_rotation_matrix;
  // Creating the lookat matrix
  lookat_matrix =
      glm::lookAt(glm::vec3(c_pos), glm::vec3(0.0), glm::vec3(c_up));

  // Creating the projection matrix
  if (enable_perspective) {
    projection_matrix = glm::frustum(-1.0, 1.0, -1.0, 1.0, 1.0, 7.0);
    // projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,5.0);
  } else {
    projection_matrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -2.0, 5.0);
  }

  view_matrix = projection_matrix * lookat_matrix;

  matrixStack.push_back(view_matrix);

  // render the roads in the RoadNetwork
  rn->renderRoads();
  // render the RoadSeps in the RoadNetwork
  rn->renderRoadSeps();
  rn->renderIntersections();

  m->executeManager();
  m->renderManager();

}

void deleteBuffersGL() {
  // delete g;
  // delete m;
  delete rn;
}

int main(int argc, char **argv) {
  // The pointer to the GLFW window
  GLFWwindow *window;

  // Setting up the GLFW Error callback
  glfwSetErrorCallback(soc::error_callback);

  // Initialize GLFW
  if (!glfwInit())
    return -1;

  // We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // We don't want the old OpenGL
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(1440, 900, "Road Network", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  // Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    // Problem: glewInit failed, something is seriously wrong.
    std::cerr << "GLEW Init Failed : %s" << std::endl;
  }

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

  std::string file_name;
  file_name = "1.raw";
  if (argc > 1) {
    file_name = argv[1];
  }

  // Initialize GL state
  soc::initGL();
  initBuffersGL(file_name);
  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0) {
    // Render here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGL();
    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();

    sleep(1);
  }

  deleteBuffersGL();

  glfwTerminate();
  return 0;
}
