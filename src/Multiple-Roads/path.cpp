#include <fstream>

#include "road_network/Multiple-Roads/path.hpp"

namespace soc {

Paths::Paths() {
  path_number = 0;

  positions.push_back(std::vector<glm::vec2>(0));
  input_status = true;

  std::string vertex_shader_file(
      "./src/Multiple-Roads/vertex-shaders/v_bezier.glsl");
  std::string fragment_shader_file(
      "./src/Multiple-Roads/fragment-shaders/f_bezier.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(
      soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = soc::CreateProgramGL(shaderList);
  glUseProgram(shaderProgram);
  v_position = glGetAttribLocation(shaderProgram, "vPosition");

  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(v_position);
  glVertexAttribPointer(v_position, 2, GL_FLOAT, GL_FALSE, 0, (void *)(0));
}

std::vector<glm::vec2> Paths::bezier_curve_point(std::vector<glm::vec2> pos,
                                                 float ratio) {
  // For logic goto
  //   https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Higher-order_curves
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

float Paths::distance(glm::vec2 &a, glm::vec2 &b) {
  return sqrt(((a[0] - b[0]) * (a[0] - b[0])) +
              ((a[1] - b[1]) * (a[1] - b[1])));
}

int Paths::interpolate_count() {
  float tot_dis = 0.0;
  for (int i = 0; i < int(positions[path_number].size()) - 1; i++) {
    tot_dis +=
        distance(positions[path_number][i], positions[path_number][i + 1]);
  }
  return int(tot_dis / AIPD);
}

void Paths::positionsToCurve() {
  // Prints all the control points given by user
  // for (int i = 0; i < positions[path_number].size(); i++) {
  //   std::cout << positions[path_number][i][0] << ", "
  //             << positions[path_number][i][1] << std::endl;
  // }

  // Stores the newly processed Bezier Curve interpolated points
  current.clear();
  float n = interpolate_count() + 1;
  if (!positions[path_number].empty()) {
    // std::cout << n << "AAA\n";
    for (float i = 0; i <= n; i++) {
      std::vector<glm::vec2> pos =
          bezier_curve_point(positions[path_number], (i / n));
      current.push_back(pos[0]);
    }
  }

  // Prints all the interpolated points
  // for (int i = 0; i < current.size(); i++) {
  //   std::cout << current[int(i)][0] << "\\"
  //             << current[int(i)][1] << std::endl;
  // }
  // std::cout <<"\n\n";
}

void Paths::getPoints(GLFWwindow *window) {
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

  positions[path_number].push_back(glm::vec2(xpos, ypos));

  // Need this as a click lasts few milliseconds
  usleep(200000);

  positionsToCurve();
}

void Paths::renderLine() {
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glUseProgram(shaderProgram);
  glBindVertexArray(vao);
  glm::vec2 cur_[current.size()];
  for (int i = 0; i < current.size(); i++) {
    cur_[i] = current[i];
  }
  glBufferData(GL_ARRAY_BUFFER, sizeof(cur_), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cur_), cur_);

  // Joins all the interpolated points, thus creating the approx Bezier Curve
  for (int i = 0; i < int(current.size()) - 1; i++) {
    glDrawArrays(GL_LINE_STRIP, i, 2);
  }
}

void Paths::next() {
  if (int(positions.size()) - 1 > path_number) {
    path_number++;

    // for (int i = 0; i < positions[path_number].size(); i++) {
    //   std::cout << positions[path_number][i][0] << ", "
    //             << positions[path_number][i][1] << std::endl;
    // }

    positionsToCurve();

    // std::cout << "10\n" << path_number << "\n";

    stop();

    return;
  }

  // for (int i = 0; i < positions[path_number].size(); i++) {
  //   std::cout << positions[path_number][i][0] << ", "
  //             << positions[path_number][i][1] << std::endl;
  // }

  positions.push_back(std::vector<glm::vec2>(0));
  current.clear();

  path_number++;

  // std::cout << "11\n" << path_number << "\n";

  resume();
}

void Paths::previous() {
  if (path_number == 0) {
    stop();
    return;
  }

  path_number--;

  // for (int i = 0; i < positions[path_number].size(); i++) {
  //   std::cout << positions[path_number][i][0] << ", "
  //             << positions[path_number][i][1] << std::endl;
  // }

  positionsToCurve();

  // std::cout << "12\n" << path_number << "\n";

  stop();
}

void Paths::delete_last() {
  if (positions[path_number].size() > 0) {
    positions[path_number].pop_back();
    positionsToCurve();
  }

  return;
}

void Paths::save() {
  std::cout << "Saving CP\n";

  std::fstream fp;
  fp.open("./models/Bezier-Model/1.min.raw", std::ios::binary | std::ios::out);

  if (!fp.good()) {
    std::cout << "could not read from the min raw file" << std::endl;
    return;
  }

  int cpsize = 0;
  for (int i = 0; i < positions.size(); i++) {
    cpsize += (positions[i].size() + 1);
  }

  // std::cout << cpsize << "BBB\n";
  glm::vec2 storecp[cpsize + 1];

  int countcp = 0;

  storecp[countcp] = glm::vec2(int(positions.size()), 0);
  countcp++;

  for (int i = 0; i < positions.size(); i++) {
    storecp[countcp] = glm::vec2(int(positions[i].size()), 0);
    countcp++;
    for (int j = 0; j < positions[i].size(); j++) {
      storecp[countcp] = positions[i][j];
      countcp++;
    }
  }

  fp.write((char *)&storecp, sizeof(storecp));
  fp.close();

  std::cout << "Saving IP\n";

  fp.open("./models/Bezier-Model/1.raw", std::ios::binary | std::ios::out);

  int save_pn = path_number;

  int ipsize = 0;
  // std::cout << positions.size() << "SSS\n";
  for (int i = 0; i < positions.size(); i++) {
    // std::cout << i << "@@";
    path_number = i;
    positionsToCurve();
    ipsize += (current.size() + 1);
  }

  // std::cout << "\n" << ipsize << "CCC\n";
  glm::vec2 storeip[ipsize + 1];

  int countip = 0;
  storeip[countip] = glm::vec2(int(positions.size()), 0);
  countip++;

  for (int i = 0; i < positions.size(); i++) {
    path_number = i;
    positionsToCurve();
    storeip[countip] = glm::vec2(int(current.size()), 0);
    countip++;
    for (int j = 0; j < current.size(); j++) {
      storeip[countip] = current[j];
      countip++;
    }
  }

  path_number = save_pn;

  fp.write((char *)&storeip, sizeof(storeip));
  fp.close();
}

void Paths::load() {
  std::cout << "Loading\n";

  std::fstream fp;
  fp.open("./models/Bezier-Model/1.min.raw", std::ios::binary | std::ios::in);

  if (!fp.good()) {
    std::cout << "could not read from the min raw file" << std::endl;
    return;
  }

  positions.clear();

  glm::vec2 total_paths;
  fp.read((char *)&total_paths, sizeof(total_paths));
  for (int i = 0; i < int(total_paths[0]); i++) {
    positions.resize(i + 1);
    glm::vec2 num;
    fp.read((char *)&num, sizeof(num));
    glm::vec2 cp[int(num[0])];
    fp.read((char *)&cp, sizeof(cp));
    for (int j = 0; j < int(num[0]); j++) {
      positions[i].push_back(cp[j]);
    }
  }

  fp.close();

  path_number = 0;

  // Converts the control points to interpolated points.
  positionsToCurve();

  // Stops further input of control points.
  stop();
}

void Paths::stop() {
  // Stops further input of control points.
  input_status = false;
}

void Paths::resume() {
  // Resumes input of control points if stopped.
  input_status = true;
}

bool Paths::return_input_status() {
  // Returns input_status (As it is a private variable)
  return input_status;
}

} // End namespace soc
