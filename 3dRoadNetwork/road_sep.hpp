//
//  road_sep.hpp
//  3dRoadNetwork
//
//  Created by Aman Yadav on 22/04/20.
//  Copyright © 2020 Aman Yadav. All rights reserved.
//

#ifndef road_sep_hpp
#define road_sep_hpp

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "road.hpp"

namespace soc{
    class RoadSep{
        GLfloat x, y, z, s;
        
        GLfloat tx, ty, tz, rx, ry, rz;
        GLuint vao[2], vbo[2];
        GLuint shaderProgram, vPosition, vColor, uModelViewMatrix;
        glm::mat4 rotation;
         glm::mat4 translation;
         
         glm::vec4 v_positions[36];
         glm::vec4 v_positions_handle[10000];
         int num_vertices_handle1;
         int num_vertices_handle2;
         int num_vertices_handle3;
         glm::vec4 v_colors[36];
         glm::vec4 v_colors_handle[10000];
         int tri_idx;
         void update_matrices();
         void roadSep(glm::vec4 *positions);
         void quad(int a, int b, int c, int d, glm::vec4 *positions);
         void build_handle();
        public:
         RoadSep(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat s);
         void render();
         void change_parameters(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rx, GLfloat ry, GLfloat rz);
        
    };
}

#endif /* road_sep_hpp */
