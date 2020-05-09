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

namespace soc {

class Paths {

private:
  std::vector< std::vector<glm::vec2> > positions; // stores all the control points
  std::vector<glm::vec2> current; // for rendering current path
  bool input_status;
  int path_number;
  GLuint vb, vao;
  GLuint shaderProgram;
  GLuint v_position;
  std::vector<glm::vec2> bezier_curve_point(std::vector<glm::vec2> pos,
                                            float ratio);
  float distance(glm::vec2 &a, glm::vec2 &b);
  int interpolate_count();
  void positionsToCurve();

public:
  Paths();
  void getPoints(GLFWwindow *window);
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
