#include "road_network/Traffic-Manager/car_model.hpp"

namespace soc {

Car::Car(Graph* graph, std::vector<int> in) {
  g = graph;
  for (int i = 0; i < in.size(); i++) {
    if (i % 2) {
      std::vector<glm::vec2> temp = g->e[in[i]].path;
      for (int j = 0; j < temp.size(); i++) {
        path.push_back(temp[j]);
      }
    }
    else {
      path.push_back(g->v[in[i]].origin);
    }
  }

  for (int i = 0; i < path.size(); i++) {
    std::cout << path[i][0] << "``" << path[i][1] << std::endl;
  }
}

}
