#ifndef manager_hpp
#define manager_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Model/road_collection.hpp"
#include "road_network/Traffic-Manager/car_model.hpp"

#define LC 5

namespace soc {

class Manager {
public:
  Graph *g;
  std::vector<Car*> cars;
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
