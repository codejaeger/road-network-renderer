#include "road_network/Traffic-Manager/car_model.hpp"

namespace soc {

Car::Car(Graph* graph, std::vector<int> in) {
  g = graph;

  for (unsigned int i = 0; i < in.size(); i++) {
    if (i % 2) {
      std::vector<glm::vec2> temp = g->e[in[i]].path;
      for (int j = 0; j < temp.size(); j++) {
        path.push_back(temp[j]);
      }
    }
    else {
      path.push_back(g->v[in[i]].origin);
    }
  }

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

Car::~Car() {
  std::cout << "Car Deleted\n";
}

}
