#ifndef path_hpp
#define path_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include "road_network/shader_util.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define BZC 50

namespace soc {

class Path {

private:
  std::vector<glm::vec2> positions;
  glm::vec2 bezier_curve_positions[BZC + 1];
  bool input_status;
  GLuint vb, vao;
  GLuint shaderProgram;
  GLuint v_position, uModelViewMatrix;
  std::vector<glm::vec2> bezier_curve_point(std::vector<glm::vec2> pos,
                                            float ratio);

public:
  Path();
  void getPoints(GLFWwindow *window);
  void positionsToCurve();
  void renderLine();
  void save();
  void load();
  void resume();
  void stop();
  bool return_input_status();
};

glm::mat4 *multiply_stack(std::vector<glm::mat4>);

} // End namespace soc

#endif /* path_hpp */
