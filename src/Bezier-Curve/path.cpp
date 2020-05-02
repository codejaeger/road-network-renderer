#include <fstream>

#include "road_network/Bezier-Curve/path.hpp"

extern std::vector<glm::mat4> matrixStack;

namespace soc {

Path::Path() {
  for (int i = 0; i < (BZC + 1); i++) {
    bezier_curve_positions[i] = glm::vec2(0, 0);
  }

  input_status = true;

  std::string vertex_shader_file(
      "./src/Bezier-Curve/vertex-shaders/v_bezier.glsl");
  std::string fragment_shader_file(
      "./src/Bezier-Curve/fragment-shaders/f_bezier.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(
      soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = soc::CreateProgramGL(shaderList);
  glUseProgram(shaderProgram);
  uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
  v_position = glGetAttribLocation(shaderProgram, "vPosition");
  glGenBuffers(1, &vb);
  std::cout << vb << "~~\n";
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(bezier_curve_positions), NULL,
               GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bezier_curve_positions),
                  bezier_curve_positions);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(v_position);
  glVertexAttribPointer(v_position, 2, GL_FLOAT, GL_FALSE, 0, (void *)(0));
}

std::vector<glm::vec2> Path::bezier_curve_point(std::vector<glm::vec2> pos,
                                                float ratio) {
  if (pos.size() == 1)
    return pos;

  std::vector<glm::vec2> new_pos;
  glm::vec2 temp;
  for (int i = 0; i < (pos.size() - 1); i++) {
    temp = glm::vec2((pos[i][0] * (1 - ratio)) + (pos[i + 1][0] * ratio),
                     (pos[i][1] * (1 - ratio)) + (pos[i + 1][1] * ratio));
    new_pos.push_back(temp);
  }

  return bezier_curve_point(new_pos, ratio);
}

void Path::getPoints(GLFWwindow *window) {
  /* Get the postition of the mouse-click w.r.t the top-left corner */
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  // display size
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  /* Converting mouse coordinates to normalized floats */
  float xpos = -1.0f + 2 * x / width;
  float ypos = +1.0f - 2 * y / height;
  std::cout << xpos << "~~" << ypos << "\n";

  positions.push_back(glm::vec2(xpos, ypos));

  /* Need this as a click lasts few milliseconds */
  usleep(200000);

  positionsToCurve();
}

void Path::positionsToCurve() {
  /* Prints all the control points given by user */
  for (int i = 0; i < positions.size(); i++) {
    std::cout << positions[i][0] << ", " << positions[i][1] << std::endl;
  }

  /* Stores the newly processed Bezier Curve interplotaed points */
  float n = BZC;
  for (float i = 0; i <= n; i++) {
    std::vector<glm::vec2> pos = bezier_curve_point(positions, (i / n));
    // std::cout << pos[0][0] << "\\\n";
    bezier_curve_positions[int(i)] = pos[0];
  }

  for (int i = 0; i < BZC + 1; i++) {
    std::cout << bezier_curve_positions[int(i)][0] << "\\" << bezier_curve_positions[int(i)][1] << std::endl;
  }
  std::cout <<"\n\n";
}

void Path::renderLine() {
  glUseProgram(shaderProgram);
  glm::mat4 *ms_mult = multiply_stack(matrixStack);
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
  glBindVertexArray(vao);
  glBufferData(GL_ARRAY_BUFFER, sizeof(bezier_curve_positions), NULL,
               GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bezier_curve_positions),
                  bezier_curve_positions);

  /* Joins all the interpolated points, thus creating the approx Bezier Curve */
  for (int i = 0; i < BZC; i++) {
    glDrawArrays(GL_LINE_STRIP, i, 2);
  }
}

void Path::save() {
  std::cout << vb << "\n";
  std::cout << "save\n";
  std::fstream fp;
  fp.open("./models/Bezier-Model/1.raw", std::ios::binary | std::ios::out);
  int n = positions.size();
  glm::vec2 store[n + 1];
  store[0] = glm::vec2(n, 0);
  for (int i = 0; i < n; i++) {
    store[i+1] = positions[i];
  }
  fp.write((char*)&store, sizeof(store));
  fp.close();
}

void Path::load() {
  std::cout << vb << "\n";
  std::cout << "load\n";
  std::fstream fp;
  fp.open("./models/Bezier-Model/1.raw", std::ios::binary | std::ios::in);

  glm::vec2 num;
  fp.read((char*)&num, sizeof(glm::vec2));
  std::cout << num[0] <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
  glm::vec2 load[int(num[0])];
  std::cout << fp.tellg() <<"\n";
  fp.read((char*)&load, sizeof(load));
  positions.clear();
  for (int i = 0; i < num[0]; i++) {
    positions.push_back(load[i]);
  }
  fp.close();

  positionsToCurve();
  input_status = false;
}

void Path::stop() {
  input_status = false;
}

void Path::restart() {
  input_status = true;
}

bool Path::return_input_status() {
  return input_status;
}

} // End namespace soc
