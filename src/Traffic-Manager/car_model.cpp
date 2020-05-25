#include "road_network/Traffic-Manager/car_model.hpp"

namespace soc {

Car::Car(Graph* graph, std::vector<int> in) {
  g = graph;

  path.clear();
  check_loc.clear();

  std::cout << "X\n";
  for (unsigned int i = 0; i < in.size(); i++) {
    if (i % 2) {
      std::vector<glm::vec2> temp = g->e[in[i]].path;
      for (unsigned int j = 0; j < temp.size(); j++) {
        path.push_back(temp[j]);
      }
      if (int(temp.size()) != 0) {
        glm::vec2 temp2 = temp.back();
        check_loc.push_back(temp2);
      }
    }
    else {
      path.push_back(g->v[in[i]].origin);
    }
  }

  if (check_loc.size())
    check_loc.pop_back();

  for (unsigned int i = 0; i < path.size(); i++) {
    std::cout << path[i][0] << "``" << path[i][1] << std::endl;
  }

  current = 0;
}

bool Car::updateCar() {
  current++;

  if (path.size() <= current + 1) {
    return false;
  }

  tangent = glm::vec2(path[current+1][0] - path[current][0], path[current+1][1] - path[current][1]);
  normal = glm::vec2(path[current+1][1] - path[current+1][1], path[current+1][0] - path[current][0]);
  std::cout << "Car Updated\n";

  return true;
}

void Car::renderCar() {
  std::cout << path[current][0] << "//" << path[current][1] << "\n";
}

glm::vec2 Car::getLocation() {
  return path[current];
}

bool Car::doCheck() {
  for (unsigned int i = 0; i < check_loc.size(); i++) {
    if (check_loc[i] == path[current])
      return true;
  }

  return false;
}

Car::~Car() {
  std::cout << "Car Deleted\n";
}

}
