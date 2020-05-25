#ifndef road_main_hpp
#define road_main_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "road_network/Road-Model/gl_framework.hpp"
#include "road_network/Road-Model/road_collection.hpp"
#include "road_network/Traffic-Manager/car_model.hpp"
#include "road_network/shader_util.hpp"

GLfloat c_xpos = 0.0, c_ypos = 0.0, c_zpos = 2.0;
GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;
GLfloat c_xrot = 0.0, c_yrot = 0.0, c_zrot = 0.0;

// Running variable to toggle culling on/off
bool enable_culling = true;
// Running variable to toggle wireframe/solid modelling
bool solid = true;
// Enable/Disable perspective view
bool enable_perspective = false;
// Shader program attribs
std::vector<glm::mat4> matrixStack;

#endif /* road_main_hpp */
