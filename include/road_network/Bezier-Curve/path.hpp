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

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// Average Interpolation Distance
#define AIPD (1.0 / 30.0)

namespace soc {

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
  std::vector<glm::vec2> bezier_curve_point(std::vector<glm::vec2> pos,
                                            float ratio);
  float distance(glm::vec2 &a, glm::vec2 &b);
  int interpolate_count(unsigned int i);
  void positionsToCurve(unsigned int i);

public:
  Paths();
  void getPoints(GLFWwindow *window);
  void renderLine(unsigned int i);
  void renderAllLines();
  void renderLine();
  void next();
  void previous();
  void delete_last();
  void save();
  void load();
  void stop();
  void resume();
  bool return_input_status();
};

} // End namespace soc

#endif /* path_hpp */
