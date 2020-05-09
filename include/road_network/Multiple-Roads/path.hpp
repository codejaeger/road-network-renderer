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

class Paths {

private:
  std::vector< std::vector<glm::vec2> > positions;
  std::vector<glm::vec2 ( * ) [BZC + 1]> bezier_curve_positions;
  glm::vec2 current_bzc[BZC + 1];
  std::vector<glm::vec2> current_cp;
  bool input_status;
  int path_number;
  GLuint vb, vao;
  GLuint shaderProgram;
  GLuint v_position;
  std::vector<glm::vec2> bezier_curve_point(std::vector<glm::vec2> pos,
                                            float ratio);

public:
  Paths();
  void getPoints(GLFWwindow *window);
  void positionsToCurve();
  void renderLine();
  void next();
  void previous();
  void save();
  void load();
  void resume();
  void stop();
  bool return_input_status();
};

} // End namespace soc

#endif /* path_hpp */
