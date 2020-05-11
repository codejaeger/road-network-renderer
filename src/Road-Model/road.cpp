#include "road_network/Road-Model/road.hpp"
#include "road_network/Road-Model/gl_framework.hpp"

extern std::vector<glm::mat4> matrixStack;
extern double PI;

namespace soc {
// Constructor that takes coordinates of the four points and depth and dist from
// where to start laying the cylindrical bars in the border as parameters
Road::Road(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, float d,
           float start_space_b, float start_space_a) {
  // set the road corner points and road depth
  x1 = p1[0];
  y1 = p1[1];
  x2 = p2[0];
  y2 = p2[1];
  x3 = p3[0];
  y3 = p3[1];
  x4 = p4[0];
  y4 = p4[1];
  depth = d;
  start_spacing_a = start_space_a;
  start_spacing_b = start_space_b;
  num_vertices = 60;
  num_vertices_border_count = 0;
  num_vertices_per_cylinder_count = 0;
  num_cylinders = 0;
  change_parameters(0, 0, 0, 0, 0, 0);

  // create the shader Program
  std::string vertex_shader_file("./src/Road-Model/vertex-shaders/v_road.glsl");
  std::string fragment_shader_file(
      "./src/Road-Model/fragment-shaders/f_road.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(
      soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = soc::CreateProgramGL(shaderList);
  glUseProgram(shaderProgram);

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation(shaderProgram, "vPosition");
  uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
  texCoord = glGetAttribLocation(shaderProgram, "texCoord");

  // coordinates of the vertices of the cuboid that represents road
  // for the purpose of having half of the side faces textured using brick
  // and half using road 4 more points are taken
  glm::vec4 positions[12] = {
      glm::vec4(x1, y1, d / 2, 1.0),  glm::vec4(x2, y2, d / 2, 1.0),
      glm::vec4(x3, y3, d / 2, 1.0),  glm::vec4(x4, y4, d / 2, 1.0),
      glm::vec4(x1, y1, 0, 1.0),      glm::vec4(x2, y2, 0, 1.0),
      glm::vec4(x3, y3, 0, 1.0),      glm::vec4(x4, y4, 0, 1.0),
      glm::vec4(x1, y1, -d / 2, 1.0), glm::vec4(x2, y2, -d / 2, 1.0),
      glm::vec4(x3, y3, -d / 2, 1.0), glm::vec4(x4, y4, -d / 2, 1.0)};

  glm::vec2 t_coords[4] = {glm::vec2(0.0, 0.0), glm::vec2(0.0, 1.0),
                           glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0)};

  tri_idx = 0;

  // Load Textures
  tex1 = LoadTexture("./data/Road-Model/btex.bmp", 526, 512);
  tex2 = LoadTexture("./data/Road-Model/rtex.bmp", 600, 340);
  glBindTexture(GL_TEXTURE_2D, tex1);
  glGenVertexArrays(1, &vao);
  // Ask GL for Vertex Buffer Object(vbo)
  glGenBuffers(1, &vbo);

  // Set 0 as the current array to be used by binding it
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // fill in the vertex positions and texture coodinates of the road
  road(positions, t_coords);

  glBufferData(GL_ARRAY_BUFFER, sizeof(v_positions) + sizeof(tex_coords), NULL,
               GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(tex_coords),
                  tex_coords);

  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(texCoord);
  glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(sizeof(v_positions)));

  // Road border
  tri_idx = 0;
  float bw = calc_dist(x1, y1, x2, y2) / 6;
  float dist1, dist2;
  dist1 = calc_dist(x1, y1, x2, y2);
  dist2 = calc_dist(x3, y3, x4, y4);
  // calculate the 4 terminal points of the border using section formula
  k1 = calc_point(x2, y2, dist1, x1, y1, bw);
  k2 = calc_point(x1, y1, dist1, x2, y2, bw);
  k3 = calc_point(x4, y4, dist2, x3, y3, bw);
  k4 = calc_point(x3, y3, dist2, x4, y4, bw);

  // generate the 4 rails of the border
  border(k1, p1, p4, k4, -d / 2);
  border(k1, p1, p4, k4, 5 * d / 4);
  border(p2, k2, k3, p3, -d / 2);
  border(p2, k2, k3, p3, 5 * d / 4);
  // generate the cylindrical bars with mutual spacing between centers as 0.08
  cylinders(2.0f);
  // creating the shader program using the vertex and fragment shaders
  std::string vertex_shader_file1(
      "./src/Road-Model/vertex-shaders/v_roadsep.glsl");
  std::string fragment_shader_file1(
      "./src/Road-Model/fragment-shaders/f_roadsep.glsl");

  std::vector<GLuint> shaderList1;
  shaderList1.push_back(
      soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file1));
  shaderList1.push_back(
      soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file1));

  shaderProgram1 = soc::CreateProgramGL(shaderList1);
  glUseProgram(shaderProgram1);

  // getting the attributes from the shader program
  vPosition1 = glGetAttribLocation(shaderProgram1, "vPosition");
  uModelViewMatrix1 = glGetUniformLocation(shaderProgram1, "uModelViewMatrix");
  vColor1 = glGetAttribLocation(shaderProgram1, "vColor");
  glGenVertexArrays(1, &vao_border);
  glGenBuffers(1, &vbo_border);

  glBindVertexArray(vao_border);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_border);
  // Adding data to the buffer
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(v_positions_border) + sizeof(v_colors_border), NULL,
               GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions_border),
                  v_positions_border);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions_border),
                  sizeof(v_colors_border), v_colors_border);
  // setting the attributes
  glEnableVertexAttribArray(vPosition1);
  glVertexAttribPointer(vPosition1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vColor1);
  glVertexAttribPointer(vColor1, 4, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(sizeof(v_positions_border)));
}

Road::~Road() {
  // free the textures from the memory
  // FreeTexture(tex1);
  // FreeTexture(tex2);
}

void Road::update_matrices() {
  // set upt the rotation and translation matrices of this road object
  rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx),
                         glm::vec3(1.0f, 0.0f, 0.0f));
  rotation =
      glm::rotate(rotation, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
  rotation =
      glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

  translation = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
}

void Road::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx,
                             GLfloat ary, GLfloat arz) {
  tx = atx;
  ty = aty;
  tz = atz;
  rx = arx;
  ry = ary;
  rz = arz;

  update_matrices();
}

void Road::render() {
  glUseProgram(shaderProgram);
  matrixStack.push_back(translation);
  matrixStack.push_back(rotation);
  // matrixStack multiply
  glm::mat4 *ms_mult = multiply_stack(matrixStack);
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
  // draw the upper part of the road having the road texture
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindTexture(GL_TEXTURE_2D, tex2);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 30);

  // draw the lower part of the road having the brick texture
  glBindTexture(GL_TEXTURE_2D, tex1);
  glDrawArrays(GL_TRIANGLES, 30, 30);

  // setting up things for drawing cuboidal slabs of the border
  glUseProgram(shaderProgram1);
  glUniformMatrix4fv(uModelViewMatrix1, 1, GL_FALSE, glm::value_ptr(*ms_mult));
  glBindBuffer(GL_ARRAY_BUFFER, vbo_border);
  glBindVertexArray(vao_border);
  glDrawArrays(GL_TRIANGLES, 0, num_vertices_border_count);

  // render the cylindrical bars of the border
  for (int i = 0; i < num_cylinders; i++) {
    glDrawArrays(GL_TRIANGLE_FAN, num_vertices_border_count +
                                      i * num_vertices_per_cylinder_count,
                 num_vertices_per_cylinder_count);
  }
  matrixStack.pop_back();
  matrixStack.pop_back();
  // for memory
  delete ms_mult;
}

// adds vertices corresponding to one face in the v_positions array
void Road::quad(int a, int b, int c, int d, glm::vec4 *positions,
                glm::vec2 *t_coords) {
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

// call the quad function to fill veritices corresponging to all the faces
// of the road
void Road::road(glm::vec4 *positions, glm::vec2 *t_coords) {
  quad(1, 0, 3, 2, positions, t_coords);
  quad(3, 7, 6, 2, positions, t_coords);
  quad(2, 6, 5, 1, positions, t_coords);
  quad(1, 5, 4, 0, positions, t_coords);
  quad(0, 4, 7, 3, positions, t_coords);
  quad(5, 9, 8, 4, positions, t_coords);
  quad(4, 8, 11, 7, positions, t_coords);
  quad(7, 11, 10, 6, positions, t_coords);
  quad(6, 10, 9, 5, positions, t_coords);
  quad(8, 9, 10, 11, positions, t_coords);
}

// create one cuboidal slab of the border
void Road::border(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d, float z) {
  glm::vec4 positions[8] = {glm::vec4(a[0], a[1], z + depth / 4, 1.0),
                            glm::vec4(b[0], b[1], z + depth / 4, 1.0),
                            glm::vec4(c[0], c[1], z + depth / 4, 1.0),
                            glm::vec4(d[0], d[1], z + depth / 4, 1.0),
                            glm::vec4(a[0], a[1], z, 1.0),
                            glm::vec4(b[0], b[1], z, 1.0),
                            glm::vec4(c[0], c[1], z, 1.0),
                            glm::vec4(d[0], d[1], z, 1.0)};

  fill_border(1, 0, 3, 2, positions);
  fill_border(2, 3, 7, 6, positions);
  fill_border(3, 0, 4, 7, positions);
  fill_border(6, 5, 1, 2, positions);
  fill_border(4, 5, 6, 7, positions);
  fill_border(5, 4, 0, 1, positions);
}

// create one face of the cuboidal slab of the border
void Road::fill_border(int a, int b, int c, int d, glm::vec4 *positions) {
  v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0);
  v_positions_border[tri_idx] = positions[a];
  tri_idx++;
  v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0);
  v_positions_border[tri_idx] = positions[b];
  tri_idx++;
  v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0);
  v_positions_border[tri_idx] = positions[c];
  tri_idx++;
  v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0);
  v_positions_border[tri_idx] = positions[a];
  tri_idx++;
  v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0);
  v_positions_border[tri_idx] = positions[c];
  tri_idx++;
  v_colors_border[tri_idx] = glm::vec4(1.0, 1.0, 1.0, 1.0);
  v_positions_border[tri_idx] = positions[d];
  tri_idx++;
  num_vertices_border_count += 6;
}

// draw the cylinders in between the cuboidal slabs of the border
void Road::cylinders(float d) {
  float r = calc_dist(x1, y1, x2, y2) / 50;
  float div = depth / 5;
  float sec = PI / 5;

  // Cylinders on the bottom part of the road
  float mpx1 = (x1 + k1[0]) / 2, mpy1 = (y1 + k1[1]) / 2;
  float mpx2 = (x4 + k4[0]) / 2, mpy2 = (y4 + k4[1]) / 2;
  float dist = calc_dist(mpx1, mpy1, mpx2, mpy2);
  float distance;
  for (distance = start_spacing_b; distance < dist; distance += d) {
    glm::vec2 v = int_div(mpx1, mpy1, mpx2, mpy2, distance / (dist - distance));

    for (float i = -depth / 4; i <= 5 * depth / 4; i += div) {
      for (float j = 0; j <= 2 * PI; j += sec) {
        v_positions_border[tri_idx] =
            glm::vec4(v[0] + r * sin(j), v[1] + r * cos(j), i, 1.0f);
        v_colors_border[tri_idx] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        tri_idx++;
        float next;
        if (i + div > 5 * depth / 4)
          next = 5 * depth / 4;
        else
          next = i + div;
        v_positions_border[tri_idx] =
            glm::vec4(v[0] + r * sin(j), v[1] + r * cos(j), next, 1.0f);
        v_colors_border[tri_idx] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        tri_idx++;
        if (distance == start_spacing_b)
          num_vertices_per_cylinder_count += 2;
      }
    }
    num_cylinders++;
  }

  end_spacing_b = distance - dist;

  // cylinders for the upper part of the road
  mpx1 = (x2 + k2[0]) / 2;
  mpy1 = (y2 + k2[1]) / 2;
  mpx2 = (x3 + k3[0]) / 2;
  mpy2 = (y3 + k3[1]) / 2;
  dist = calc_dist(mpx1, mpy1, mpx2, mpy2);

  for (distance = start_spacing_a; distance < dist; distance += d) {
    glm::vec2 v = int_div(mpx1, mpy1, mpx2, mpy2, distance / (dist - distance));
    for (float i = -depth / 4; i <= 5 * depth / 4; i += div) {
      for (float j = 0; j <= 2 * PI; j += sec) {
        v_positions_border[tri_idx] =
            glm::vec4(v[0] + r * sin(j), v[1] + r * cos(j), i, 1.0f);
        v_colors_border[tri_idx] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        tri_idx++;
        float next;
        if (i + div > 5 * depth / 4)
          next = 5 * depth / 4;
        else
          next = i + div;
        v_positions_border[tri_idx] =
            glm::vec4(v[0] + r * sin(j), v[1] + r * cos(j), next, 1.0f);
        v_colors_border[tri_idx] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        tri_idx++;
      }
    }

    num_cylinders++;
  }

  end_spacing_a = distance - dist;
  // store the distance at which the next road object should begin laying
  // the cylindrical bars
}

// function to calculate distance between two points
float Road::calc_dist(float xa, float ya, float xb, float yb) {
  return sqrt((xa - xb) * (xa - xb) + (ya - yb) * (ya - yb));
}

glm::vec2 Road::calc_point(float xa, float ya, float dist1, float xb, float yb,
                           float dist2) {
  float a, b;
  float r = dist2 / dist1;
  a = (1 + r) * xb - r * xa;
  b = (1 + r) * yb - r * ya;
  return glm::vec2(a, b);
}

// calculate the point which divides the two given points
// internally in the ratio r
glm::vec2 Road::int_div(float xa, float ya, float xb, float yb, float r) {
  float a, b;
  a = (r * xb + xa) / (1 + r);
  b = (r * yb + ya) / (1 + r);
  return glm::vec2(a, b);
}

// multiply all the elements of the stack matrix
glm::mat4 *multiply_stack(std::vector<glm::mat4> matStack) {
  glm::mat4 *mult;
  mult = new glm::mat4(1.0f);

  for (int i = 0; i < matStack.size(); i++) {
    *mult = (*mult) * matStack[i];
  }

  return mult;
}

} // End namespace soc
