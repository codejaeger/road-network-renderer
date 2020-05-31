#ifndef output_main_hpp
#define output_main_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <unistd.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "road_network/output_gl_framework.hpp"
#include "road_network/shader_util.hpp"
#include "road_network/Camera/camera.hpp"
#include "road_network/Road-Model/road_collection.hpp"
#include "road_network/Traffic-Manager/manager.hpp"
#include "road_network/SkyMaps/skymaps.hpp"

GLfloat xrot = -45.0, yrot = 0.0, zrot = 0.0;
// Shader program attribs
std::vector<glm::mat4> matrixStack;

#endif /* output_main_hpp */
