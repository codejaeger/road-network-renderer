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
public:
  Graph *g;
  std::vector<CarNode*> cars;
  std::vector<IntersectionLights*> lights;
  std::vector<unsigned int> e_no_go;
  unsigned int time;
  unsigned int frame_rate;
  unsigned int light_timeout;
  unsigned int car_spawnin;
  int start;
  int end;
  Manager(Graph* graph, int s, int e);
  ~Manager();
  void executeManager();
  void renderManager();

};


}

#endif
