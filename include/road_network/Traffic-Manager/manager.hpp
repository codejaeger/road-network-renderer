#ifndef manager_hpp
#define manager_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Graph/graph.hpp"

#include "road_network/Traffic-Manager/car_node.hpp"
#include "road_network/Traffic-Manager/intersection_lights.hpp"

#define LC 5

namespace soc {

class Manager {
private:
  Graph *g;
  std::vector<CarNode*> cars;
  std::vector<IntersectionLights*> lights;
  std::vector<glm::vec2> edge_firsts_go;
  unsigned int time, frame_rate, light_timeout, light_timeout_green, light_timeout_yellow;
  unsigned int car_spawnin;
  std::vector<int> starts;
  std::vector<int> ends;
  unsigned int spawn_flag;
public:
  Manager(Graph* graph, std::string file_name);
  ~Manager();
  void executeManager();
  void renderManager();

};

}

#endif /* manager_hpp */
