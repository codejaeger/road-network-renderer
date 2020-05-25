#ifndef manager_hpp
#define manager_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Graph/graph.hpp"
#include "road_network/Traffic-Manager/car_model.hpp"
#include "road_network/Traffic-Manager/traffic_light.hpp"

#define LC 5

namespace soc {

class Manager {
public:
  Graph *g;
  std::vector<Car*> cars;
  std::vector<Light*> lights;
  std::vector<unsigned int> e_no_go;
  unsigned int time;
  int start;
  int end;
  Manager(Graph* graph, int s, int e);
  ~Manager();
  void executeManager();
  void renderManager();

};


}

#endif
