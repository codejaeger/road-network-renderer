#ifndef road_hpp
#define road_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "road_network/Road-Model/gl_framework.hpp"
#include "road_network/Road-Model/texture.hpp"
#include "road_network/shader_util.hpp"

namespace soc {

class Road {

private:
  // rotation and translation parameters
  GLfloat tx, ty, tz, rx, ry, rz;
  // depth of the road
  GLfloat depth;
  //whether to draw above and below borders or not
  bool a_border, b_border;
  // coordinates of the four corners of the road projected on x-y plane
  GLfloat x1, y1, x2, y2, x3, y3, x4, y4;
  // points where road-border ends
  glm::vec2 k1, k2, k3, k4;
  // store the road and brick textures
  GLuint tex1, tex2;
  // distance from one end of the road
  // where the cylindrical bars are supposed to start from
  float start_spacing_b, start_spacing_a;
  GLuint num_vertices;
  GLuint vao, vbo;
  GLuint vao_border, vbo_border;
  GLuint shaderProgram1, vPosition1, vColor1, uModelViewMatrix1;
  GLuint shaderProgram, vPosition, texCoord, uModelViewMatrix;
  glm::mat4 rotation;
  glm::mat4 translation;
  glm::vec4 v_positions[60];
  glm::vec2 tex_coords[60];
  glm::vec4 v_positions_border[100000];
  glm::vec4 v_colors_border[100000];
  int num_vertices_border_count;
  int num_vertices_per_cylinder_count;
  int num_cylinders, num_cylinders_b, num_cylinders_a;
  int tri_idx;
  void update_matrices();
  void road(glm::vec4 *positions, glm::vec2 *t_coords);
  void quad(int a, int b, int c, int d, glm::vec4 *positions,
            glm::vec2 *t_coords);
  void border(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d, float z);
  void fill_border(int a, int b, int c, int d, glm::vec4 *positions);
  void cylinders(float dist);
  float calc_dist(float xa, float ya, float xb, float yb);
  glm::vec2 calc_point(float xa, float ya, float dist1, float xb, float yb,
                       float dist2);
  glm::vec2 int_div(float xa, float ya, float xb, float yb, float ratio);

public:
  float end_spacing_b, end_spacing_a;
  Road(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, GLfloat depth,
       float start_spacing_b, float start_spacing_a, bool b=true, bool a=true);
  ~Road();
  void render();
  void change_parameters(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rx,
                         GLfloat ry, GLfloat rz);
};

glm::mat4 *multiply_stack(std::vector<glm::mat4>);

} // End namespace soc

#endif /* road_hpp */
