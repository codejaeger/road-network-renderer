#include "road_network/Bezier-Curve/start_end.hpp"

namespace soc {

Pairs::Pairs(std::vector< std::vector<glm::vec2> > positions) {
  for (unsigned int i = 0; i < positions.size(); i++) {
    if (positions[i].size()) {
      path_terminals.push_back(positions[i][0]);
      path_terminals.push_back(positions[i].back());
    }
  }

  is_start = true;
  start.clear();
  end.clear();

  if (path_terminals.size() == 0) {
    save();
  }

  // Shaders
  std::string vertex_shader_file(
      "./src/Bezier-Curve/vertex-shaders/v_bezier.glsl");
  std::string fragment_shader_file_start(
      "./src/Bezier-Curve/fragment-shaders/f_start.glsl");
  std::string fragment_shader_file_end(
      "./src/Bezier-Curve/fragment-shaders/f_end.glsl");

  std::vector<GLuint> shaderList_start;
  shaderList_start.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList_start.push_back(
      soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file_start));
  shaderProgram_start = soc::CreateProgramGL(shaderList_start);
  glUseProgram(shaderProgram_start);
  v_position_start = glGetAttribLocation(shaderProgram_start, "vPosition");

  std::vector<GLuint> shaderList_end;
  shaderList_end.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList_end.push_back(
      soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file_end));
  shaderProgram_end = soc::CreateProgramGL(shaderList_end);
  glUseProgram(shaderProgram_end);
  v_position_end = glGetAttribLocation(shaderProgram_end, "vPosition");

  // Create buffer which is used to render paths
  glGenBuffers(1, &vb_start);
  glBindBuffer(GL_ARRAY_BUFFER, vb_start);
  glGenVertexArrays(1, &vao_start);
  glBindVertexArray(vao_start);
  glEnableVertexAttribArray(v_position_start);
  glVertexAttribPointer(v_position_start, 2, GL_FLOAT, GL_FALSE, 0, (void *)(0));

  glGenBuffers(1, &vb_end);
  glBindBuffer(GL_ARRAY_BUFFER, vb_end);
  glGenVertexArrays(1, &vao_end);
  glBindVertexArray(vao_end);
  glEnableVertexAttribArray(v_position_end);
  glVertexAttribPointer(v_position_end, 2, GL_FLOAT, GL_FALSE, 0, (void *)(0));
}

unsigned int Pairs::findNearest(glm::vec2 pos) {
  float min_distance = 10.0;
  unsigned int index = -1;
  for (unsigned int i = 0; i < path_terminals.size(); i++) {
    float curr_dis = distance(pos, path_terminals[i]);
    if (min_distance > curr_dis) {
      index = i;
      min_distance = curr_dis;
    }
  }
  return index;
}

void Pairs::getPoints(GLFWwindow *window) {
  // Get the postition of the mouse-click w.r.t the top-left corner
  double x, y;
  glfwGetCursorPos(window, &x, &y);

  // Display size
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  // Converting mouse coordinates to normalized floats
  float xpos = -1.0f + 2 * x / width;
  float ypos = +1.0f - 2 * y / height;
  std::cout << xpos << "~~" << ypos << "\n";

  if (is_start) {
    start.push_back(path_terminals[findNearest(glm::vec2(xpos, ypos))]);
  }
  else {
    end.push_back(path_terminals[findNearest(glm::vec2(xpos, ypos))]);
  }

  is_start = !is_start;

  // Need this as a click lasts few milliseconds
  usleep(200000);
}

void Pairs::delete_last() {
  if (is_start) {
    if (end.size())
      end.pop_back();
  }
  else {
    if (start.size())
      start.pop_back();
  }
  is_start = !is_start;
}

void Pairs::renderPoints(std::vector<glm::vec2> &ren) {
  glm::vec2 change[4] = {
    glm::vec2( 0.1,  0.0),
    glm::vec2(-0.1,  0.0),
    glm::vec2( 0.0,  0.1),
    glm::vec2( 0.0, -0.1),
  };
  glm::vec2 cur[4 * ren.size()];
  for (unsigned int i = 0; i + 1 < 4 * ren.size(); i++) {
    cur[i] = ren[i/4] + change[i%4];
  }

  // Buffering the array to GPU
  glBufferData(GL_ARRAY_BUFFER, sizeof(cur), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cur), cur);

  for (unsigned int j = 0; j + 1 < 4 * ren.size(); j = j + 2) {
    glDrawArrays(GL_LINE_STRIP, j, 2);
  }
}

void Pairs::renderAllPoints() {
  glBindBuffer(GL_ARRAY_BUFFER, vb_start);
  glUseProgram(shaderProgram_start);
  glBindVertexArray(vao_start);
  renderPoints(start);

  glBindBuffer(GL_ARRAY_BUFFER, vb_end);
  glUseProgram(shaderProgram_end);
  glBindVertexArray(vao_end);
  renderPoints(end);
}

void Pairs::save() {
  std::cout << "Start\n";
  for (unsigned int i = 0; i < start.size(); i++) {
    std::cout << start[i][0] << "//" << start[i][1] << "\n";
  }
  std::cout << "End\n";
  for (unsigned int i = 0; i < end.size(); i++) {
    std::cout << end[i][0] << "//" << end[i][1] << "\n";
  }
}

}
