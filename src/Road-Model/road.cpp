//
//  road.cpp
//  3dRoadNetwork
//
//  Created by Aman Yadav on 17/04/20.
//  Copyright © 2020 Aman Yadav. All rights reserved.
//

#include "road_network/Road-Model/road.hpp"
#include <iostream>
extern std::vector<glm::mat4> matrixStack;
extern double PI;
namespace soc {
Road::Road(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, GLfloat d, float start_space_b, float start_space_a){
        x1 = p1[0]; y1 = p1[1]; x2 = p2[0]; y2 = p2[1];
        x3 = p3[0]; y3 = p3[1]; x4 = p4[0]; y4 = p4[1];
        depth = d;
        start_spacing_a = start_space_a;
        start_spacing_b = start_space_b;
        num_vertices = 60;
        num_vertices_border_count=0;
        num_vertices_per_cylinder_count=0;
        num_cylinders=0;
        change_parameters(0, 0, 0, 0, 0, 0);
            //Eight vertices in homogenous coordinates
         std::string vertex_shader_file("./src/Road-Model/vertex-shaders/v_road.glsl");
         std::string fragment_shader_file("./src/Road-Model/fragment-shaders/f_road.glsl");

         std::vector<GLuint> shaderList;
         shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
         shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

         shaderProgram = csX75::CreateProgramGL(shaderList);
         glUseProgram( shaderProgram );

        // getting the attributes from the shader program
         vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
         uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
         texCoord = glGetAttribLocation( shaderProgram, "texCoord" );

        glm::vec4 positions[12] = {
          glm::vec4(x1, y1, d/2, 1.0),
          glm::vec4(x2, y2, d/2, 1.0),
          glm::vec4(x3, y3, d/2, 1.0),
          glm::vec4(x4, y4, d/2, 1.0),
          glm::vec4(x1, y1, 0, 1.0),
          glm::vec4(x2, y2, 0, 1.0),
          glm::vec4(x3, y3, 0, 1.0),
          glm::vec4(x4, y4, 0, 1.0),
          glm::vec4(x1, y1, -d/2, 1.0),
          glm::vec4(x2, y2, -d/2, 1.0),
          glm::vec4(x3, y3, -d/2, 1.0),
          glm::vec4(x4, y4, -d/2, 1.0)
        };

        glm::vec2 t_coords[4] = {
          glm::vec2( 0.0, 0.0),
          glm::vec2( 0.0, 1.0),
          glm::vec2( 1.0, 0.0),
          glm::vec2( 1.0, 1.0)
        };
        

        tri_idx=0;
        
        // Load Textures
        tex1=LoadTexture("./data/btex.bmp",526, 512);
        tex2=LoadTexture("./data/rtex.bmp", 600, 340);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glGenVertexArrays (1, &vao);
        //Ask GL for two Vertex Buffer Object (vbo)
        glGenBuffers (1, &vbo);

        //Set 0 as the current array to be used by binding it
        glBindVertexArray (vao);
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo);

        road(positions, t_coords);


        glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions );
        glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(tex_coords), tex_coords);

        glEnableVertexAttribArray( vPosition );
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
        //Textures
        glEnableVertexAttribArray( texCoord );
        glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)) );
        
        //Road border
            tri_idx = 0;
    int bw = d;
    int dist1, dist2;
    dist1 = calc_dist(x1, y1, x2, y2);
    dist2 = calc_dist(x3, y3, x4, y4);
    k1=calc_point(x2, y2, dist1, x1, y1, bw); k2 = calc_point(x1, y1, dist1, x2, y2, bw); k3 = calc_point(x4, y4, dist2, x3, y3, bw);
    k4 = calc_point(x3, y3, dist2, x4, y4, bw);
    
    border(k1, p1, p4, k4, -d/2);
    border(k1, p1, p4, k4, 5*d/4);
    border(p2, k2, k3, p3, -d/2);
    border(p2, k2, k3, p3, 5*d/4);
    cylinders(1.0f);
     std::string vertex_shader_file1("./src/Road-Model/vertex-shaders/v_roadsep.glsl");
     std::string fragment_shader_file1("./src/Road-Model/fragment-shaders/f_roadsep.glsl");

     std::vector<GLuint> shaderList1;
     shaderList1.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file1));
     shaderList1.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file1));

     shaderProgram1 = csX75::CreateProgramGL(shaderList1);
     glUseProgram( shaderProgram1 );

    // getting the attributes from the shader program
     vPosition1 = glGetAttribLocation( shaderProgram1, "vPosition" );
     uModelViewMatrix1 = glGetUniformLocation( shaderProgram1, "uModelViewMatrix");
     vColor1 = glGetAttribLocation( shaderProgram1, "vColor" );
    glGenVertexArrays (1, &vao_border);
    //Ask GL for two Vertex Buffer Object (vbo)
    glGenBuffers (1, &vbo_border);

    //Set 0 as the current array to be used by binding it
    glBindVertexArray (vao_border);
    
    glBindBuffer (GL_ARRAY_BUFFER, vbo_border);

    glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions_border) + sizeof(v_colors_border), NULL, GL_STATIC_DRAW);
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions_border), v_positions_border );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions_border), sizeof(v_colors_border), v_colors_border);

    glEnableVertexAttribArray( vPosition1 );
    glVertexAttribPointer( vPosition1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    //Textures
    glEnableVertexAttribArray( vColor1 );
    glVertexAttribPointer( vColor1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions_border)) );
    }

    void Road::update_matrices(){

        rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f,0.0f,0.0f));
        rotation = glm::rotate(rotation, glm::radians(ry), glm::vec3(0.0f,1.0f,0.0f));
        rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f,0.0f,1.0f));

        translation = glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,tz));


    }

    void Road::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz){
        tx = atx;
        ty = aty;
        tz = atz;
        rx = arx;
        ry = ary;
        rz = arz;

        update_matrices();
    }

    void Road::render(){
        glUseProgram(shaderProgram);
        matrixStack.push_back(translation);
        matrixStack.push_back(rotation);
        //matrixStack multiply
        glm::mat4* ms_mult = multiply_stack(matrixStack);
        glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
       // glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindTexture(GL_TEXTURE_2D, tex2);
        glBindVertexArray (vao);
        glDrawArrays(GL_TRIANGLES, 0, 30);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glDrawArrays(GL_TRIANGLES, 30, 30);;
        
        //draw border
        glUseProgram(shaderProgram1);
        glUniformMatrix4fv(uModelViewMatrix1, 1, GL_FALSE, glm::value_ptr(*ms_mult));
        glBindVertexArray (vao_border);
        glDrawArrays(GL_TRIANGLES, 0, num_vertices_border_count);

        for(int i =0; i<num_cylinders; i++){
            glDrawArrays(GL_TRIANGLE_FAN, num_vertices_border_count + i * num_vertices_per_cylinder_count, num_vertices_per_cylinder_count);
        }
        matrixStack.pop_back();
        matrixStack.pop_back();
        // for memory
        delete ms_mult;

    }
    void Road::quad(int a, int b, int c, int d, glm::vec4 *positions, glm::vec2 *t_coords)
    {
      v_positions[tri_idx] = positions[a];
      tex_coords[tri_idx] = t_coords[1];
      tri_idx++;
      v_positions[tri_idx] = positions[b];
      tex_coords[tri_idx] = t_coords[0];
      tri_idx++;
      v_positions[tri_idx] = positions[c];
      tex_coords[tri_idx] = t_coords[2];
      tri_idx++;
      v_positions[tri_idx] = positions[a];
      tex_coords[tri_idx] = t_coords[1];
      tri_idx++;
      v_positions[tri_idx] = positions[c];
      tex_coords[tri_idx] = t_coords[2];
      tri_idx++;
      v_positions[tri_idx] = positions[d];
      tex_coords[tri_idx] = t_coords[3];
      tri_idx++;
     }

     void Road::road(glm::vec4 *positions, glm::vec2 *t_coords )
     {
          quad( 1, 0, 3, 2, positions, t_coords);
          quad( 3, 7, 6, 2,  positions, t_coords);
          quad( 2, 6, 5, 1, positions, t_coords);
          quad( 1, 5, 4, 0, positions, t_coords);
          quad( 0, 4, 7 ,3, positions, t_coords);
          quad( 5, 9, 8, 4, positions, t_coords);
          quad( 4, 8, 11, 7, positions, t_coords);
          quad( 7, 11, 10, 6, positions, t_coords);
          quad( 6, 10, 9, 5, positions, t_coords);
          quad( 8, 9, 10, 11, positions, t_coords);
    }

    void Road::border(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d, float z){
        glm::vec4 positions[8]={
            glm::vec4(a[0], a[1], z+depth/4, 1.0),
            glm::vec4(b[0], b[1], z+depth/4, 1.0),
            glm::vec4(c[0], c[1], z+depth/4, 1.0),
            glm::vec4(d[0], d[1], z+depth/4, 1.0),
            glm::vec4(a[0], a[1], z, 1.0),
            glm::vec4(b[0], b[1], z, 1.0),
            glm::vec4(c[0], c[1], z, 1.0),
            glm::vec4(d[0], d[1], z, 1.0)
        };
        fill_border( 1, 0, 3, 2, positions );
        fill_border( 2, 3, 7, 6, positions );
        fill_border( 3, 0, 4, 7, positions );
        fill_border( 6, 5, 1, 2, positions );
        fill_border( 4, 5, 6, 7, positions );
        fill_border( 5, 4, 0, 1, positions );
    }
    void Road::fill_border(int a, int b, int c, int d, glm::vec4 *positions)
    {
      v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0); v_positions_border[tri_idx] = positions[a]; tri_idx++;
      v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0); v_positions_border[tri_idx] = positions[b]; tri_idx++;
      v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0); v_positions_border[tri_idx] = positions[c]; tri_idx++;
      v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0); v_positions_border[tri_idx] = positions[a]; tri_idx++;
      v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0); v_positions_border[tri_idx] = positions[c]; tri_idx++;
      v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0); v_positions_border[tri_idx] = positions[d]; tri_idx++;
      num_vertices_border_count+=6;
                                                        
     }
void Road::cylinders(float d){
        float r = 0.1f;
        float div = depth/5;
        float sec = PI / 5;
    //cylinders on the bottom part of the road
    float mpx1 = (x1 + k1[0])/2, mpy1 = (y1 + k1[1])/2;
    float mpx2 = (x4 + k4[0])/2, mpy2 = (y4 + k4[1])/2;
    float dist = calc_dist(mpx1, mpy1, mpx2, mpy2);
    float distance;
    for(distance = start_spacing_b; distance < dist; distance+=d){
        glm::vec2 v = int_div(mpx1, mpy1, mpx2, mpy2, distance/(dist - distance));

       for(float i = -depth/4; i<=5*depth/4; i = i + div ){
        for(float j =0; j<= 2 * PI; j = j + sec ){
            v_positions_border[tri_idx] = glm::vec4(v[0] + r * sin(j), v[1] + r*cos(j), i, 1.0f);
            v_colors_border[tri_idx] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            tri_idx++;
            float next;
            if(i + div > 5*depth/4)
               next = 5*depth/4;
            else next = i+div;
            v_positions_border[tri_idx] = glm::vec4(v[0] + r * sin(j), v[1] + r*cos(j), next, 1.0f);
            v_colors_border[tri_idx] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            tri_idx++;
            if(distance == start_spacing_b)
                num_vertices_per_cylinder_count+=2;
        }
       }
        num_cylinders++;
    }
    end_spacing_b = distance - dist;

    //cylinders for the upper part of the road
    mpx1 = (x2 + k2[0])/2; mpy1 = (y2 + k2[1])/2;
    mpx2 = (x3 + k3[0])/2; mpy2 = (y3 + k3[1])/2;
    dist = calc_dist(mpx1, mpy1, mpx2, mpy2);

    for(distance = start_spacing_a; distance < dist; distance+=d){
        glm::vec2 v = int_div(mpx1, mpy1, mpx2, mpy2, distance/(dist - distance));
       for(float i = -depth/4; i<=5*depth/4; i = i + div ){
        for(float j =0; j<= 2 * PI; j = j + sec ){
            v_positions_border[tri_idx] = glm::vec4(v[0] + r * sin(j), v[1] + r*cos(j), i, 1.0f);
            v_colors_border[tri_idx] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            tri_idx++;
            float next;
            if(i + div > 5*depth/4)
               next = 5*depth/4;
            else next = i+div;
            v_positions_border[tri_idx] = glm::vec4(v[0] + r * sin(j), v[1] + r*cos(j), next, 1.0f);
            v_colors_border[tri_idx] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            tri_idx++;        }
       }
        num_cylinders++;
    }
    end_spacing_a = distance - dist;
}
    float Road::calc_dist(float xa, float ya, float xb, float yb){
        return sqrt((xa-xb)*(xa-xb) + (ya-yb)*(ya-yb));
    }

    glm::vec2 Road::calc_point(float xa, float ya, float dist1, float xb, float yb,float dist2){
        float a, b;
        float r = dist2/dist1;
        a = (1 + r) * xb - r * xa;
        b = (1 + r) * yb - r * ya;
        return glm::vec2(a, b);
    }
    
    glm::vec2 Road::int_div(float xa, float ya, float xb, float yb, float r){
        float a, b;
        a = (r * xb + xa)/(1 + r);
        b = (r* yb + ya)/(1 + r);
        return glm::vec2(a, b);
    }
    glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
        glm::mat4* mult;
        mult = new glm::mat4(1.0f);
    
        for(int i=0;i<matStack.size();i++){
            *mult = (*mult) * matStack[i];
        }

        return mult;
    }
    
}
