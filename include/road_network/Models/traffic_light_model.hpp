#ifndef traffic_light_hpp
#define traffic_light_hpp

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
#include "road_network/Road-Model/road.hpp"
#include "road_network/shader_util.hpp"

namespace soc {

class TrafficLightModel {
private:
  // x, y, z => coordinates of the center of the base, size =>scaling factor
  GLfloat x, y, z, s;
  // rotation and translation parameters
  GLfloat tx, ty, tz, rx, ry, rz;
  GLuint vao, vbo;
  GLuint shaderProgram, vPosition, vColor, uModelViewMatrix;
  glm::mat4 rotation;
  glm::mat4 translation;
  glm::vec4 v_positions[108];
  glm::vec4 v_colors[108];
  int tri_idx;
  void update_matrices();
  void trafficLight(glm::vec4 *positions, glm::vec4 *colors);
  void quad(int a, int b, int c, int d, glm::vec4 *positions, glm::vec4 color);
public:
  TrafficLightModel(GLfloat size);
  void render();
  void change_parameters(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rx,
                         GLfloat ry, GLfloat rz);
  void turnYellow();
  void turnGreen();
  void turnRed();
};

} // End namespace soc

#endif /* road_sep_hpp */
