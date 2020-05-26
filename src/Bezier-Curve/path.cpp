#include <fstream>

#include "road_network/Bezier-Curve/path.hpp"

namespace soc {

Paths::Paths() {
  // Path number initialized
  path_number = 0;

  // First path initialized
  positions.push_back(std::vector<glm::vec2>(0));
  bzc.push_back(std::vector<glm::vec2>(0));

  // Input of control point starts
  input_status = true;

  // Shaders
  std::string vertex_shader_file(
      "./src/Bezier-Curve/vertex-shaders/v_bezier.glsl");
  std::string fragment_shader_file_current(
      "./src/Bezier-Curve/fragment-shaders/f_current.glsl");
  std::string fragment_shader_file_rest(
      "./src/Bezier-Curve/fragment-shaders/f_rest.glsl");

  std::vector<GLuint> shaderList_current;
  shaderList_current.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList_current.push_back(
      soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file_current));
  shaderProgram_current = soc::CreateProgramGL(shaderList_current);
  glUseProgram(shaderProgram_current);
  v_position_current = glGetAttribLocation(shaderProgram_current, "vPosition");

  std::vector<GLuint> shaderList_rest;
  shaderList_rest.push_back(soc::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList_rest.push_back(
      soc::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file_rest));
  shaderProgram_rest = soc::CreateProgramGL(shaderList_rest);
  glUseProgram(shaderProgram_rest);
  v_position_rest = glGetAttribLocation(shaderProgram_rest, "vPosition");

  // Create buffer which is used to render paths
  glGenBuffers(1, &vb_current);
  glBindBuffer(GL_ARRAY_BUFFER, vb_current);
  glGenVertexArrays(1, &vao_current);
  glBindVertexArray(vao_current);
  glEnableVertexAttribArray(v_position_current);
  glVertexAttribPointer(v_position_current, 2, GL_FLOAT, GL_FALSE, 0, (void *)(0));

  glGenBuffers(1, &vb_rest);
  glBindBuffer(GL_ARRAY_BUFFER, vb_rest);
  glGenVertexArrays(1, &vao_rest);
  glBindVertexArray(vao_rest);
  glEnableVertexAttribArray(v_position_rest);
  glVertexAttribPointer(v_position_rest, 2, GL_FLOAT, GL_FALSE, 0, (void *)(0));
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
  // Basic distance formula
  return sqrt(((a[0] - b[0]) * (a[0] - b[0])) +
              ((a[1] - b[1]) * (a[1] - b[1])));
}

int Paths::interpolate_count() {
  // Calculating interpolation counts
  // It controls the distance between interpolation points
  float tot_dis = 0.0;
  for (int i = 0; i < int(positions[path_number].size()) - 1; i++) {
    tot_dis +=
        distance(positions[path_number][i], positions[path_number][i + 1]);
  }
  return int(tot_dis / AIPD);
}

void Paths::positionsToCurve() {
  // Prints all the control points given by user for the current path
  // for (int i = 0; i < positions[path_number].size(); i++) {
  //   std::cout << positions[path_number][i][0] << ", "
  //             << positions[path_number][i][1] << std::endl;
  // }

  // Stores the newly processed Bezier Curve interpolated points
  bzc[path_number].clear();
  float n = interpolate_count() + 1; // +1 is to avoid unexpected things
  if (!positions[path_number].empty()) {
    for (float i = 0; i <= n; i++) {
      std::vector<glm::vec2> pos =
          bezier_curve_point(positions[path_number], (i / n));
      bzc[path_number].push_back(pos[0]);
    }
  }

  // Prints all the interpolated points for the current path
  // for (int i = 0; i < bzc[path_number].size(); i++) {
  //   std::cout << bzc[path_number][int(i)][0] << "\\"
  //             << bzc[path_number][int(i)][1] << std::endl;
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

  // Converts the control points to interpolated points.
  positionsToCurve();
}

void Paths::renderLine(unsigned int i) {
  // Convert the vector storing interpolated points to array
  glm::vec2 cur[bzc[i].size()];
  for (unsigned int j = 0; j < bzc[i].size(); j++) {
    cur[j] = bzc[i][j];
  }

  // Buffering the array to GPU
  glBufferData(GL_ARRAY_BUFFER, sizeof(cur), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cur), cur);

  // Joins consecutive interpolated points
  // Creates the approx Bezier Curve
  for (unsigned int j = 0; j + 1 < bzc[i].size(); j++) {
    glDrawArrays(GL_LINE_STRIP, j, 2);
  }
}

void Paths::renderAllLines() {
  positionsToCurve();
  for (unsigned int i = 0; i < bzc.size(); i++) {
    // Bind the buffer for rendering path
    if (i == path_number) {
      glBindBuffer(GL_ARRAY_BUFFER, vb_current);
      glUseProgram(shaderProgram_current);
      glBindVertexArray(vao_current);
    }
    else {
      glBindBuffer(GL_ARRAY_BUFFER, vb_rest);
      glUseProgram(shaderProgram_rest);
      glBindVertexArray(vao_rest);
    }
    renderLine(i);
  }
}

void Paths::next() {
  // If the next path already exists
  if (positions.size() > path_number + 1) {
    stop();
  }
  // If the next path doesn't exist then create it and
  else {
    positions.push_back(std::vector<glm::vec2>(0));
    bzc.push_back(std::vector<glm::vec2>(0));
    resume();
  }

  // Move to next path number
  path_number++;

  // Update the vector storing path's interpolated points
  positionsToCurve();
}

void Paths::previous() {
  // If you are not at path 1
  if (path_number != 0) {
    path_number--;
  }
  stop();
}

void Paths::delete_last() {
  // If there is a contol point in this path, then delete the last one.
  if (positions[path_number].size() > 0) {
    positions[path_number].pop_back();
  }
}

void Paths::save() {
  std::fstream fp;
  int size, count;

  // Open .min.raw file
  std::cout << "Saving 1.min.raw\n";
  fp.open("./models/Bezier-Model/1.min.raw", std::ios::binary | std::ios::out);

  // Check file's condition
  if (!fp.good()) {
    std::cout << "could not read from the min raw file" << std::endl;
    return;
  }

  // Find the size of array of "values to store"
  size = 0;
  for (int i = 0; i < positions.size(); i++) {
    size += (positions[i].size() + 1);
  }

  // Generate an array of that size
  glm::vec2 storecp[size + 1];

  // Start the counter
  count = 0;

  // First, stores the number of paths
  storecp[count] = glm::vec2(int(positions.size()), 0);
  count++;

  // Recursively stores the number of points then the points
  for (int i = 0; i < positions.size(); i++) {
    storecp[count] = glm::vec2(int(positions[i].size()), 0);
    count++;
    for (int j = 0; j < positions[i].size(); j++) {
      storecp[count] = positions[i][j];
      count++;
    }
  }

  // Write to the file, then close it
  fp.write((char *)&storecp, sizeof(storecp));
  fp.close();


  // Open .raw file
  std::cout << "Saving 1.raw\n";
  fp.open("./models/Bezier-Model/1.raw", std::ios::binary | std::ios::out);

  // Check file's condition
  if (!fp.good()) {
    std::cout << "could not read from the min raw file" << std::endl;
    return;
  }

  // Find the size of array of "values to store"
  size = 0;
  for (int i = 0; i < bzc.size(); i++) {
    size += (bzc[i].size() + 1);
  }

  // Generate an array of that size
  glm::vec2 storeip[size + 1];

  // Start the counter
  count = 0;

  // First, stores the number of paths
  storeip[count] = glm::vec2(int(bzc.size()), 0);
  count++;

  // Recursively stores the number of points then the points
  for (int i = 0; i < bzc.size(); i++) {
    storeip[count] = glm::vec2(int(bzc[i].size()), 0);
    count++;
    for (int j = 0; j < bzc[i].size(); j++) {
      storeip[count] = bzc[i][j];
      count++;
    }
  }

  // Write to the file, then close it
  fp.write((char *)&storeip, sizeof(storeip));
  fp.close();
}

void Paths::load() {
  std::fstream fp;

  // Open .min.raw file
  std::cout << "Loading\n";
  fp.open("./models/Bezier-Model/1.min.raw", std::ios::binary | std::ios::in);

  // Check file's condition
  if (!fp.good()) {
    std::cout << "could not read from the min raw file" << std::endl;
    return;
  }

  // Clear the vector storing control points
  positions.clear();

  // Find the number of paths
  glm::vec2 total_paths;
  fp.read((char *)&total_paths, sizeof(total_paths));

  // Recursively find the number of points then store the points
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

  // Close the file
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
