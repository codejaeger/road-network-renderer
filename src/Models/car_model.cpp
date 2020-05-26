#include "road_network/Models/car_model.hpp"

extern std::vector<glm::mat4> matrixStack;

namespace soc {

CarModel::CarModel(GLfloat size) {
  s = size;
  change_parameters(0, 0, 0, 0, 0, 0);
  // Creating the shader Program from teh vertex and fragment shaders..
  std::string vertex_shader_file(
      "./src/Road-Model/vertex-shaders/v_roadsep.glsl");
  std::string fragment_shader_file(
      "./src/Road-Model/fragment-shaders/f_roadsep.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = soc::CreateProgramGL(shaderList);
  glUseProgram(shaderProgram);

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation(shaderProgram, "vPosition");
  uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
  vColor = glGetAttribLocation(shaderProgram, "vColor");

  glm::vec4 positions1[8] = {glm::vec4(-1 * s, -0.5 * s, 0, 1.0),
                            glm::vec4(-1 * s, 0.5 * s, 0, 1.0),
                            glm::vec4(1 * s, 0.5 * s, 0, 1.0),
                            glm::vec4(1 * s, -0.5 * s, 0, 1.0),
                            glm::vec4(-1 * s, -0.5 * s, 0.25 * s, 1.0),
                            glm::vec4(-1 * s, 0.5 * s, 0.25 * s, 1.0),
                            glm::vec4(1 * s, 0.5 * s, 0.25 * s, 1.0),
                            glm::vec4(1 * s, -0.5 * s, 0.25 * s, 1.0)};
  srand(time(0));
  glm::vec4 color_b((rand()%256)/255.0, (rand()%256)/255.0, (rand()%256)/255.0, 1.0f);
  glm::vec4 colors1[6] ={color_b, color_b, color_b, color_b, color_b, color_b};

  // the vertices of the frustum shaped base
  glm::vec4 positions2[8] = {glm::vec4(-0.85 * s, -0.45 * s, 0.25 * s, 1.0),
                            glm::vec4(-0.85 * s, 0.45 * s, 0.25 * s, 1.0),
                            glm::vec4(0.55 * s, 0.45 * s, 0.25 * s, 1.0),
                            glm::vec4(0.55 * s, -0.45 * s, 0.25 * s, 1.0),
                            glm::vec4(-0.75 * s, -0.35 * s, 0.5 * s, 1.0),
                            glm::vec4(-0.75 * s, 0.35 * s, 0.5 * s, 1.0),
                            glm::vec4(0.45 * s, 0.35 * s, 0.5 * s, 1.0),
                            glm::vec4(0.45 * s, -0.35 * s, 0.5 * s, 1.0)};
  glm::vec4 color_glass(1.0f, 0.647f, 0.0f, 1.0f);
  glm::vec4 colors2[6] = {color_glass, color_glass, color_glass, color_glass, color_glass, glm::vec4(abs(color_b[0]-0.1), abs(color_b[1]-0.1), abs(color_b[2]-0.1), 1.0f)};

  glm::vec4 positions3[8] = {glm::vec4(-0.90 * s, -0.25 * s, 0.25 * s, 1.0),
                            glm::vec4(-0.90 * s, 0.25 * s, 0.25 * s, 1.0),
                            glm::vec4(-0.85 * s, 0.25 * s, 0.25 * s, 1.0),
                            glm::vec4(-0.85 * s, -0.25 * s, 0.25 * s, 1.0),
                            glm::vec4(-0.90 * s, -0.25 * s, 0.35 * s, 1.0),
                            glm::vec4(-0.90 * s, 0.25 * s, 0.35 * s, 1.0),
                            glm::vec4(-0.85 * s, 0.25 * s, 0.35 * s, 1.0),
                            glm::vec4(-0.85 * s, -0.25 * s, 0.35 * s, 1.0)};

  glm::vec4 Red(1.0f, 0.0f, 0.0f, 1.0f);
  glm::vec4 colors3[6] = {Red, Red, Red, Red, Red, Red};

  glm::vec4 positions4[8] = {glm::vec4(1 * s, -0.4 * s, 0.075 * s, 1.0),
                            glm::vec4(1 * s, -0.3 * s, 0.075 * s, 1.0),
                            glm::vec4(1.05 * s, -0.3 * s, 0.075 * s, 1.0),
                            glm::vec4(1.05 * s, -0.4 * s, 0.075 * s, 1.0),
                            glm::vec4(1 * s, -0.4 * s, 0.175 * s, 1.0),
                            glm::vec4(1 * s, -0.3 * s, 0.175 * s, 1.0),
                            glm::vec4(1.05 * s, -0.3 * s, 0.175 * s, 1.0),
                            glm::vec4(1.05 * s, -0.4 * s, 0.175 * s, 1.0)};

  glm::vec4 Yellow(1.0f, 0.647f, 0.0f, 1.0f);
  glm::vec4 colors4[6] = {Yellow, Yellow, Yellow, Yellow, Yellow, Yellow};

  glm::vec4 positions5[8] = {glm::vec4(1 * s, 0.3 * s, 0.075 * s, 1.0),
                            glm::vec4(1 * s, 0.4 * s, 0.075 * s, 1.0),
                            glm::vec4(1.05 * s, 0.4 * s, 0.075 * s, 1.0),
                            glm::vec4(1.05 * s, 0.3 * s, 0.075 * s, 1.0),
                            glm::vec4(1 * s, 0.3 * s, 0.175 * s, 1.0),
                            glm::vec4(1 * s, 0.4 * s, 0.175 * s, 1.0),
                            glm::vec4(1.05 * s, 0.4 * s, 0.175 * s, 1.0),
                            glm::vec4(1.05 * s, 0.3 * s, 0.175 * s, 1.0)};


  tri_idx = 0;
  // Ask GL for vertex array object(vao)
  glGenVertexArrays(1, &vao);
  // Ask GL for Vertex Buffer Object(vbo)
  glGenBuffers(1, &vbo);

  // Set 0 as the current array to be used by binding it
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // fill up the vertices  in the positions array
  car(positions1, colors1);
  car(positions2, colors2);
  car(positions3, colors3);
  car(positions4, colors4);
  car(positions5, colors4);

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
}

void CarModel::update_matrices() {
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

void CarModel::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz,
                                GLfloat arx, GLfloat ary, GLfloat arz) {
  tx = atx;
  ty = aty;
  tz = atz;
  rx = arx;
  ry = ary;
  rz = arz;

  update_matrices();
}

void CarModel::render() {
  glUseProgram(shaderProgram);
  matrixStack.push_back(translation);
  matrixStack.push_back(rotation);
  // matrixStack multiply
  glm::mat4 *ms_mult = multiply_stack(matrixStack);
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
  // render the base of the separator
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 180);
  // pop the pushed rotation and translation matrices
  matrixStack.pop_back();
  matrixStack.pop_back();
  // for memory
  delete ms_mult;
}

void CarModel::quad(int a, int b, int c, int d, glm::vec4 *positions, glm::vec4 color) {
  v_positions[tri_idx] = positions[a];
  v_colors[tri_idx] = color;
  tri_idx++;
  v_positions[tri_idx] = positions[b];
  v_colors[tri_idx] = color;
  tri_idx++;
  v_positions[tri_idx] = positions[c];
  v_colors[tri_idx] = color;
  tri_idx++;
  v_positions[tri_idx] = positions[a];
  v_colors[tri_idx] = color;
  tri_idx++;
  v_positions[tri_idx] = positions[c];
  v_colors[tri_idx] =  color;
  tri_idx++;
  v_positions[tri_idx] = positions[d];
  v_colors[tri_idx] = color;
  tri_idx++;
}


void CarModel::car(glm::vec4 *positions, glm::vec4 *colors) {
  quad(1, 0, 3, 2, positions, colors[0]);
  quad(3, 7, 6, 2, positions, colors[1]);
  quad(2, 6, 5, 1, positions, colors[2]);
  quad(1, 5, 4, 0, positions, colors[3]);
  quad(0, 4, 7, 3, positions, colors[4]);
  quad(5, 4, 7, 6, positions, colors[5]);
}

} // End namespace soc
