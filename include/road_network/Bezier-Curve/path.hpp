#ifndef path_hpp
#define path_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <math.h>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include "road_network/shader_util.hpp"
#include "road_network/constants.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace soc {

std::vector<glm::vec2> bezier_curve_point(std::vector<glm::vec2> &pos,
                                          float ratio);
float distance(glm::vec2 &a, glm::vec2 &b);
int interpolate_count(std::vector<glm::vec2> &positions);

class Paths {

private:
  // stores all the control points
  std::vector< std::vector<glm::vec2> > positions;
  // for rendering current path
  std::vector< std::vector<glm::vec2> > bzc;
  bool input_status;
  unsigned int path_number;
  GLuint vb_current, vb_rest, vao_current, vao_rest;
  GLuint shaderProgram_current, shaderProgram_rest;
  GLuint v_position_current, v_position_rest;
  void positionsToCurve(unsigned int i);

public:
  Paths();
  void getPoints(GLFWwindow *window);
  void renderLine(unsigned int i);
  void renderAllLines();
  void next();
  void previous();
  void delete_last();
  void save();
  void load();
  void stop();
  void resume();
  bool return_input_status();
  std::vector< std::vector<glm::vec2> > return_positions();
};

} // End namespace soc

#endif /* path_hpp */
