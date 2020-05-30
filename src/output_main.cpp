#include "road_network/output_main.hpp"

glm::mat4 model_view_matrix;
glm::mat4 rotation_matrix;
glm::mat4 view_matrix;
glm::mat4 projection_matrix;
glm::quat MyQuaternion;

soc::RoadNetwork *rn;
soc::Graph *g;
soc::Manager *m;
soc::SkyMaps *sm;

const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

void initBuffersGL() {
  rn = new soc::RoadNetwork(0.02, 0.05, "1.raw"); // road-depth=0.02, road-width=0.1
  rn->initRoadNetwork();
  sm = new soc::SkyMaps();
  g = rn->getGraph();
  // std::cout << g->v.size() << "ss" << g->e.size() << "\n";
  // for (unsigned int i = 0; i < g->v.size(); i++) {
  //   std::cout << g->v[i].origin[0] << "pp" << g->v[i].origin[1] << std::endl;
  // }
  m = new soc::Manager(g, "points.raw");
}

void renderGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // per-frame time logic
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  glm::vec3 EulerAngles(glm::radians(xrot), glm::radians(yrot), glm::radians(zrot));
  MyQuaternion = glm::quat(EulerAngles);
  rotation_matrix = glm::toMat4(MyQuaternion);

    projection_matrix = glm::perspective((float)glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 50.0f);
  // camera/view transformation
  glm::mat4 view = camera.GetViewMatrix();
  view_matrix = projection_matrix * view;
  model_view_matrix = view_matrix * rotation_matrix;
  matrixStack.clear();
  matrixStack.push_back(model_view_matrix);

  // render the roads in the RoadNetwork
  rn->renderRoads();
  // render the RoadSeps in the RoadNetwork
  rn->renderRoadSeps();
  rn->renderIntersections();
  sm->render();
  m->executeManager();
  m->renderManager();

}

void deleteBuffersGL() {
  delete rn;
  delete m;
  delete sm;
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
  // Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, soc::framebuffer_size_callback);
  // To ensure we can capture key being pressed
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  // To ensure we can capture mouse button being pressed
  glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
  // Cursor Position Callback
  glfwSetCursorPosCallback(window, soc::mouse_callback);
  // Mouse Scrool Callback
  glfwSetScrollCallback(window, soc::scroll_callback);
  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialize GL state
  soc::initGL();
  initBuffersGL();
  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0) {
    // Render here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGL();
    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();

    // sleep(1);
  }

  deleteBuffersGL();

  glfwTerminate();
  return 0;
}
