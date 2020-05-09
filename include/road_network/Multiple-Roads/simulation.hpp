#ifndef simulation_hpp
#define simulation_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "road_network/Multiple-Roads/gl_framework.hpp"
#include "road_network/Multiple-Roads/path.hpp"
#include "road_network/shader_util.hpp"

#define HEIGHT 900
#define WIDTH 1440

// Running variable to toggle culling on/off
bool enable_culling = true;
// Running variable to toggle wireframe/solid modelling
bool solid = true;
// Enable/Disable perspective view
bool enable_perspective = false;

#endif /* simulation_hpp */
