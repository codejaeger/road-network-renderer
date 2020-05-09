#include <fstream>

#include "road_network/Multiple-Roads/path.hpp"

namespace soc {

Paths::Paths() {
  for (int i = 0; i < (BZC + 1); i++) {
    current_bzc[i] = glm::vec2(0, 0);
  }

  path_number = 0;
  bezier_curve_positions.push_back(new glm::vec2[BZC + 1]);

  for (int i = 0; i < (BZC + 1); i++) {
    bezier_curve_positions[path_number][i] = glm::vec2(0, 0);
  }

  positions.push_back(*(new std::vector<glm::vec2>));
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(current_bzc), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(current_bzc), current_bzc);
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

void Paths::positionsToCurve() {
  // Prints all the control points given by user
  for (int i = 0; i < positions[path_number].size(); i++) {
    std::cout << positions[path_number][i][0] << ", " << positions[path_number][i][1] << std::endl;
  }

  // Stores the newly processed Bezier Curve interpolated points
  float n = BZC;
  for (float i = 0; i <= n; i++) {
    std::vector<glm::vec2> pos = bezier_curve_point(positions[path_number], (i / n));
    bezier_curve_positions[path_number][int(i)] = pos[0];
    current_bzc[int(i)] = pos[0];
  }

  // // Prints all the interpolated points
  for (int i = 0; i < BZC + 1; i++) {
    std::cout << bezier_curve_positions[path_number][int(i)][0] << "\\"
              << bezier_curve_positions[path_number][int(i)][1] << std::endl;
  }
  std::cout <<"\n\n";
}

void Paths::renderLine() {
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glUseProgram(shaderProgram);
  glBindVertexArray(vao);
  glBufferData(GL_ARRAY_BUFFER, sizeof(current_bzc), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(current_bzc), current_bzc);

  // Joins all the interpolated points, thus creating the approx Bezier Curve
  for (int i = 0; i < BZC; i++) {
    glDrawArrays(GL_LINE_STRIP, i, 2);
  }
}

void Paths::next() {
  if (positions.size() - 1 > path_number) {
    path_number++;

    for (int i = 0; i < (BZC + 1); i++) {
      current_bzc[i] = bezier_curve_positions[path_number][i];
    }

    for (int i = 0; i < BZC + 1; i++) {
      std::cout << bezier_curve_positions[path_number][int(i)][0] << "\\"
                << bezier_curve_positions[path_number][int(i)][1] << std::endl;
    }

    for (int i = 0; i < positions[path_number].size(); i++) {
      std::cout << positions[path_number][i][0] << ", " << positions[path_number][i][1] << std::endl;
    }

    std::cout << "10\n" << path_number << "\n";

    stop();

    return ;
  }

  for (int i = 0; i < (BZC + 1); i++) {
    current_bzc[i] = glm::vec2(0, 0);
  }

  for (int i = 0; i < BZC + 1; i++) {
    std::cout << bezier_curve_positions[path_number][int(i)][0] << "\\"
              << bezier_curve_positions[path_number][int(i)][1] << std::endl;
  }

  for (int i = 0; i < positions[path_number].size(); i++) {
    std::cout << positions[path_number][i][0] << ", " << positions[path_number][i][1] << std::endl;
  }

  bezier_curve_positions.push_back(new glm::vec2[BZC + 1]);

  positions.push_back(*(new std::vector<glm::vec2>));

  path_number++;

  std::cout << "11\n" << path_number << "\n";
}

// void Paths::save() {
//   std::cout << "Saving\n";
//
//   std::fstream fp;
//   fp.open("./models/Bezier-Model/1.raw", std::ios::binary | std::ios::out);
//
//   // First saves the number of elements present in the vector.
//   int n = positions.size();
//   glm::vec2 store[n + 1];
//   store[0] = glm::vec2(n, 0);
//
//   // Then stores the vector as array and write.
//   for (int i = 0; i < n; i++) {
//     store[i + 1] = positions[i];
//   }
//   fp.write((char *)&store, sizeof(store));
//
//   fp.close();
// }
//
// void Paths::load() {
//   std::cout << "Loading\n";
//
//   std::fstream fp;
//   fp.open("./models/Bezier-Model/1.raw", std::ios::binary | std::ios::in);
//
//   // First gets the number of elements of vector stored.
//   glm::vec2 num;
//   fp.read((char *)&num, sizeof(glm::vec2));
//
//   // Then creates a array to extract the elements.
//   glm::vec2 load[int(num[0])];
//   fp.read((char *)&load, sizeof(load));
//
//   // Then replaces the vector by the array data.
//   positions.clear();
//   for (int i = 0; i < num[0]; i++) {
//     positions.push_back(load[i]);
//   }
//
//   fp.close();
//
//   // Converts the control points to interpolated points.
//   positionsToCurve();
//
//   // Stops further input of control points.
//   stop();
// }

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
