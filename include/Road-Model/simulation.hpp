//
//  simulation.hpp
//  3dRoadNetwork
//
//  Created by Aman Yadav on 17/04/20.
//  Copyright © 2020 Aman Yadav. All rights reserved.
//

#ifndef simulation_hpp
#define simulation_hpp

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "road.hpp"
#include "road_sep.hpp"


GLfloat c_xpos = 0.0, c_ypos = 0.0, c_zpos = 2.0;
GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;
GLfloat c_xrot=0.0,c_yrot=0.0,c_zrot=0.0;

//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;
//Enable/Disable perspective view
bool enable_perspective=false;
//Shader program attribs
std::vector<glm::mat4> matrixStack;

//-------------------------------------------------------------------------



#endif /* simulation_hpp */
