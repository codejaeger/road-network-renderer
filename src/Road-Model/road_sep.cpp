//
//  road_sep.cpp
//  3dRoadNetwork
//
//  Created by Aman Yadav on 22/04/20.
//  Copyright © 2020 Aman Yadav. All rights reserved.
//

#include <Road-Model/road_sep.hpp>
#include <iostream>
double PI=3.14159265;
extern std::vector<glm::mat4> matrixStack;
namespace soc{
    RoadSep::RoadSep(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat size){
    
         x = x1; y = y1; z= z1; s = size;
            change_parameters(x, y, z, 0, 0, 0);
        num_vertices_handle1=0;
        num_vertices_handle2=0;
        num_vertices_handle3=0;

         std::string vertex_shader_file("./src/Road-Model/vertex-shaders/v_roadsep.glsl");
         std::string fragment_shader_file("./src/Road-Model/fragment-shaders/f_roadsep.glsl");

         std::vector<GLuint> shaderList;
         shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
         shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

         shaderProgram = csX75::CreateProgramGL(shaderList);
         glUseProgram( shaderProgram );

        // getting the attributes from the shader program
         vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
         uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
         vColor = glGetAttribLocation( shaderProgram, "vColor" );
        
            glm::vec4 positions[8] = {
              glm::vec4(-4*s, -1.75*s, 0, 1.0),
              glm::vec4(-4*s, 1.75*s, 0, 1.0),
              glm::vec4(4*s, 1.75*s, 0, 1.0),
              glm::vec4(4*s, -1.75*s, 0, 1.0),
              glm::vec4(-3.5*s, -1.5*s, s, 1.0),
              glm::vec4(-3.5*s, 1.5*s, s, 1.0),
              glm::vec4(3.5*s, 1.5*s, s, 1.0),
              glm::vec4(3.5*s, -1.5*s, s, 1.0)
            };
            tri_idx=0;

            glGenVertexArrays (1, &vao[0]);
            //Ask GL for two Vertex Buffer Object (vbo)
            glGenBuffers (1, &vbo[0]);

            //Set 0 as the current array to be used by binding it
            glBindVertexArray (vao[0]);
            
            glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);

            roadSep(positions);


            glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions) + sizeof(v_colors), NULL, GL_STATIC_DRAW);
            glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions );
            glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_colors), v_colors);

            glEnableVertexAttribArray( vPosition );
            glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
            
            glEnableVertexAttribArray( vColor );
            glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)) );
            
        
            //Handle
        build_handle();
        
        glGenVertexArrays (1, &vao[1]);
        //Ask GL for two Vertex Buffer Object (vbo)
        glGenBuffers (1, &vbo[1]);

        //Set 0 as the current array to be used by binding it
        glBindVertexArray (vao[1]);
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);

        glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions_handle) + sizeof(v_colors_handle), NULL, GL_STATIC_DRAW);
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions_handle), v_positions_handle );
        glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions_handle), sizeof(v_colors_handle), v_colors_handle);

        glEnableVertexAttribArray( vPosition );
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
        
        glEnableVertexAttribArray( vColor );
        glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions_handle)) );
        
        }

        void RoadSep::update_matrices(){

            rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f,0.0f,0.0f));
            rotation = glm::rotate(rotation, glm::radians(ry), glm::vec3(0.0f,1.0f,0.0f));
            rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f,0.0f,1.0f));

            translation = glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,tz));


        }

        void RoadSep::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz){
            tx = atx;
            ty = aty;
            tz = atz;
            rx = arx;
            ry = ary;
            rz = arz;

            update_matrices();
        }

        void RoadSep::render(){
            glUseProgram(shaderProgram);
            matrixStack.push_back(translation);
            matrixStack.push_back(rotation);
            //matrixStack multiply
            glm::mat4* ms_mult = multiply_stack(matrixStack);
            glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));

            glBindVertexArray (vao[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(vao[1]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, num_vertices_handle1);
            glDrawArrays(GL_TRIANGLE_FAN, num_vertices_handle1, num_vertices_handle2);
            glDrawArrays(GL_TRIANGLE_FAN, num_vertices_handle1+num_vertices_handle2, num_vertices_handle3);
            matrixStack.pop_back();
            matrixStack.pop_back();
            // for memory
            delete ms_mult;

        }
        void RoadSep::quad(int a, int b, int c, int d, glm::vec4 *positions)
        {
          v_positions[tri_idx] = positions[a];
          v_colors[tri_idx] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
          tri_idx++;
          v_positions[tri_idx] = positions[b];
          v_colors[tri_idx] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
          tri_idx++;
          v_positions[tri_idx] = positions[c];
          v_colors[tri_idx] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
          tri_idx++;
          v_positions[tri_idx] = positions[a];
          v_colors[tri_idx] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
          tri_idx++;
          v_positions[tri_idx] = positions[c];
          v_colors[tri_idx] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
          tri_idx++;
          v_positions[tri_idx] = positions[d];
          v_colors[tri_idx] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
          tri_idx++;
         }

         void RoadSep::roadSep(glm::vec4 *positions)
         {
              quad( 1, 0, 3, 2, positions);
              quad( 3, 7, 6, 2,  positions);
              quad( 2, 6, 5, 1, positions);
              quad( 1, 5, 4, 0, positions);
              quad( 0, 4, 7 ,3, positions);
              quad( 5, 4, 6, 7, positions);
        }
        
        void RoadSep::build_handle(){
            tri_idx = 0;
                       float r = 0.15 * s;
                       float R = 1.5*s - r;
                       float div = 4 * s/20;
                       float sec = PI / 5;
                   for(float i = s; i<=5*s; i = i + div ){
                       for(float j =0; j<= 2 * PI; j = j + sec ){
                           v_positions_handle[tri_idx] = glm::vec4(r * sin(j), R - r*cos(j), i, 1.0f);
                           v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                           tri_idx++;
                           float next;
                           if(i + div > 5*s)
                              next = 5*s;
                           else next = i+div;
                           v_positions_handle[tri_idx] = glm::vec4(r * sin(j), R - r*cos(j), next, 1.0f);
                           v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                           tri_idx++;
                           num_vertices_handle1+=2;
                       }
                   }
                   float slice = PI/50;
                   for(float i = 0.0; i<= PI; i+=slice){
                       for(float j= 0.0; j<=2*PI; j+=sec){
                           float x = r * sin(j);
                           float y = (R - r* cos(j)) * cos(i);
                           float z = (R- r*cos(j)) * sin(i) + 5*s;
                           v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                           v_positions_handle[tri_idx] = glm::vec4(x, y, z, 1.0f);
                           tri_idx++;
                           float next;
                           if(i + slice > PI)
                               next = PI;
                           else next = i + slice;
                           x = r * sin(j);
                           y = (R - r* cos(j)) * cos(next);
                           z = (R- r*cos(j)) * sin(next) + 6*s;
                           v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                           v_positions_handle[tri_idx] = glm::vec4(x, y, z, 1.0f);
                           tri_idx++;
                           num_vertices_handle2+=2;
                       }
                   }
                   
                   for(float i = s; i<=5*s; i = i + div ){
                       for(float j =0; j<= 2 * PI; j = j + sec ){
                           v_positions_handle[tri_idx] = glm::vec4(r * sin(j), -R + r*cos(j), i, 1.0f);
                           v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                           tri_idx++;
                           float next;
                           if(i + div > 5*s)
                              next = 5*s;
                           else next = i+div;
                           v_positions_handle[tri_idx] = glm::vec4(r * sin(j), -R + r*cos(j), next, 1.0f);
                           v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                           tri_idx++;
                           num_vertices_handle3+=2;
                       }
                   }
        }

}

