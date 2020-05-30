#ifndef start_end_hpp
#define start_end_hpp

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

#include "road_network/Bezier-Curve/path.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace soc {

class Pairs {

private:
  std::vector<glm::vec2> path_terminals;
  std::vector<glm::vec2> start;
  std::vector<glm::vec2> end;
  bool is_start;
  GLuint vb_start, vb_end, vao_start, vao_end;
  GLuint shaderProgram_start, shaderProgram_end;
  GLuint v_position_start, v_position_end;
  unsigned int findNearest(glm::vec2 pos);

public:
  Pairs(std::vector< std::vector<glm::vec2> > positions);
  void getPoints(GLFWwindow *window);
  void delete_last();
  void renderPoints(std::vector<glm::vec2> &ren);
  void renderAllPoints();
  void save();
  unsigned int return_path_terminals_size();
  ~Pairs();
};

} // End namespace soc

#endif /* start_end_hpp */
