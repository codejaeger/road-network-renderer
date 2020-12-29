#include "road_network/Road-Model/road_sep.hpp"

extern std::vector<glm::mat4> matrixStack;

namespace soc {

RoadSep::RoadSep(glm::vec3 v, GLfloat size) {
  // v is the coordinate of center of the base of road seperator
  x = v[0];
  y = v[1];
  z = v[2];
  s = size;
  // translate it to required coordinates
  change_parameters(x, y, z, 0, 0, 0);

  // set the number of vertices in the three parts of handle to 0
  num_vertices_handle1 = 0;
  num_vertices_handle2 = 0;
  num_vertices_handle3 = 0;

  // Creating the shader Program from teh vertex and fragment shaders..
  std::string vertex_shader_file(
      "./src/Road-Model/vertex-shaders/v_roadsep.glsl");
  std::string fragment_shader_file(
      "./src/Road-Model/fragment-shaders/f_roadsep.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(
      soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = soc::CreateProgramGL(shaderList);
  glUseProgram(shaderProgram);

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation(shaderProgram, "vPosition");
  uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
  vColor = glGetAttribLocation(shaderProgram, "vColor");

  // the vertices of the frustum shaped base
  glm::vec4 positions[8] = {glm::vec4(-4 * s, -1.75 * s, 0, 1.0),
                            glm::vec4(-4 * s, 1.75 * s, 0, 1.0),
                            glm::vec4(4 * s, 1.75 * s, 0, 1.0),
                            glm::vec4(4 * s, -1.75 * s, 0, 1.0),
                            glm::vec4(-3.5 * s, -1.5 * s, s, 1.0),
                            glm::vec4(-3.5 * s, 1.5 * s, s, 1.0),
                            glm::vec4(3.5 * s, 1.5 * s, s, 1.0),
                            glm::vec4(3.5 * s, -1.5 * s, s, 1.0)};
  tri_idx = 0;
  // Ask GL for vertex array object(vao)
  glGenVertexArrays(1, &vao[0]);
  // Ask GL for Vertex Buffer Object(vbo)
  glGenBuffers(1, &vbo[0]);

  // Set 0 as the current array to be used by binding it
  glBindVertexArray(vao[0]);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  // fill up the vertices of the base  in the positions array
  roadSep(positions);

  glBufferData(GL_ARRAY_BUFFER, sizeof(v_positions) + sizeof(v_colors), NULL,
               GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_colors),
                  v_colors);

  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(sizeof(v_positions)));

  // Handle
  build_handle();
  // Ask GL for the Vertex Array Object(vao)
  glGenVertexArrays(1, &vao[1]);
  // Ask GL for Vertex Buffer Object(vbo)
  glGenBuffers(1, &vbo[1]);

  // Set 0 as the current array to be used by binding it
  glBindVertexArray(vao[1]);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

  glBufferData(GL_ARRAY_BUFFER,
               sizeof(v_positions_handle) + sizeof(v_colors_handle), NULL,
               GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions_handle),
                  v_positions_handle);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions_handle),
                  sizeof(v_colors_handle), v_colors_handle);
  // set Attributes
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(sizeof(v_positions_handle)));
}

void RoadSep::update_matrices() {
  // set the rotation and translation matrices acc to the current values
  // of the parameters
  rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx),
                         glm::vec3(1.0f, 0.0f, 0.0f));
  rotation =
      glm::rotate(rotation, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
  rotation =
      glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

  translation = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
}

void RoadSep::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz,
                                GLfloat arx, GLfloat ary, GLfloat arz) {
  tx = atx;
  ty = aty;
  tz = atz;
  rx = arx;
  ry = ary;
  rz = arz;

  update_matrices();
}

void RoadSep::render() {
  glUseProgram(shaderProgram);
  matrixStack.push_back(translation);
  matrixStack.push_back(rotation);
  // matrixStack multiply
  glm::mat4 *ms_mult = multiply_stack(matrixStack);
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
  // render the base of the separator
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBindVertexArray(vao[0]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  // render the three handles on the top of the base
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBindVertexArray(vao[1]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, num_vertices_handle1);
  glDrawArrays(GL_TRIANGLE_FAN, num_vertices_handle1, num_vertices_handle2);
  glDrawArrays(GL_TRIANGLE_FAN, num_vertices_handle1 + num_vertices_handle2,
               num_vertices_handle3);
  // pop the pushed rotation and translation matrices
  matrixStack.pop_back();
  matrixStack.pop_back();
  // for memory
  delete ms_mult;
}

void RoadSep::quad(int a, int b, int c, int d, glm::vec4 *positions) {
  v_positions[tri_idx] = positions[a];
  v_colors[tri_idx] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // yellow colored base
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

void RoadSep::roadSep(glm::vec4 *positions) {
  quad(1, 0, 3, 2, positions);
  quad(3, 7, 6, 2, positions);
  quad(2, 6, 5, 1, positions);
  quad(1, 5, 4, 0, positions);
  quad(0, 4, 7, 3, positions);
  quad(5, 4, 7, 6, positions);
}

void RoadSep::build_handle() {
  tri_idx = 0;
  float r = 0.15 * s;
  float R = 1.5 * s - r;
  float div = 4 * s / 20;
  float sec = PI / 5;

  // create the cylindrical handle using parameteric equation of the cylinders
  for (float i = s; i <= 5 * s; i += div) {
    for (float j = 0; j <= 2 * PI; j += sec) {
      v_positions_handle[tri_idx] =
          glm::vec4(r * sin(j), R - r * cos(j), i, 1.0f);
      v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
      tri_idx++;
      float next;
      if (i + div > 5 * s)
        next = 5 * s;
      else
        next = i + div;
      v_positions_handle[tri_idx] =
          glm::vec4(r * sin(j), R - r * cos(j), next, 1.0f);
      v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
      tri_idx++;
      num_vertices_handle1 += 2;
    }
  }

  // generate the other handle of the separator
  float slice = PI / 50;
  for (float i = 0.0; i <= PI; i += slice) {
    for (float j = 0.0; j <= 2 * PI; j += sec) {
      float x = r * sin(j);
      float y = (R - r * cos(j)) * cos(i);
      float z = (R - r * cos(j)) * sin(i) + 5 * s;
      v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
      v_positions_handle[tri_idx] = glm::vec4(x, y, z, 1.0f);
      tri_idx++;
      float next;
      if (i + slice > PI)
        next = PI;
      else
        next = i + slice;
      x = r * sin(j);
      y = (R - r * cos(j)) * cos(next);
      z = (R - r * cos(j)) * sin(next) + 6 * s;
      v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
      v_positions_handle[tri_idx] = glm::vec4(x, y, z, 1.0f);
      tri_idx++;
      num_vertices_handle2 += 2;
    }
  }

  // generate the middle semicircular disc part
  for (float i = s; i <= 5 * s; i += div) {
    for (float j = 0; j <= 2 * PI; j += sec) {
      v_positions_handle[tri_idx] =
          glm::vec4(r * sin(j), -R + r * cos(j), i, 1.0f);
      v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
      tri_idx++;
      float next;
      if (i + div > 5 * s)
        next = 5 * s;
      else
        next = i + div;
      v_positions_handle[tri_idx] =
          glm::vec4(r * sin(j), -R + r * cos(j), next, 1.0f);
      v_colors_handle[tri_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
      tri_idx++;
      num_vertices_handle3 += 2;
    }
  }
}

} // End namespace soc
