#include "road_network/Traffic-Manager/car_node.hpp"

namespace soc {

CarNode::CarNode(Graph* graph, std::vector<int> in) {
  g = graph;

  path.clear(); // Stores the path
  check_loc.clear(); // Stores the points just before intersections

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

  std::cout << "creating car\n";
  for (unsigned int i = 0; i < path.size(); i++) {
    std::cout << path[i][0] << "``" << path[i][1] << std::endl;
  }
  std::cout << path.size() << "CarNode created\n";

  current = 0;
}

bool CarNode::updateCar() {
  current++;

  if (path.size() <= current + 1) {
    return false;
  }

  float x1 = path[current][0];
  float y1 = path[current][1];
  float x2 = path[current+1][0];
  float y2 = path[current+1][1];
  float nf = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)); // normalization_factor
  tangent = glm::vec2((x2-x1)/nf, (y2-y1)/nf);
  normal = glm::vec2((y2-y1)/nf, (x1-x2)/nf);
  std::cout << tangent[0] << "," << tangent[1] << "..."
            << normal[0] << "," << normal[1] << " CarNode Updated\n";

  return true;
}

void CarNode::renderCar() {
  std::cout << path[current][0] << "//" << path[current][1] << "\n";
}

glm::vec2 CarNode::getLocation() {
  return path[current];
}

glm::vec2 CarNode::getNextLocation() {
  if (path.size() <= current + 1) {
    return glm::vec2(-50, -50);  // Consider it as a dumping ground.
  }
  return path[current+1];
}

bool CarNode::doCheck() {
  for (unsigned int i = 0; i < check_loc.size(); i++) {
    if (check_loc[i] == path[current])
      return true;
  }

  return false;
}

CarNode::~CarNode() {
  std::cout << "CarNode Deleted\n";
}

}
