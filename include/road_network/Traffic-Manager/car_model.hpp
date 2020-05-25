#ifndef car_model_hpp
#define car_model_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Graph/graph.hpp"

namespace soc {

class Car	{
public:
  Graph *g;
  std::vector<glm::vec2> path;
  unsigned int current;
  std::vector<glm::vec2> check_loc;
  glm::vec2 tangent;
  glm::vec2 normal;
  Car(Graph* graph, std::vector<int> in);
  ~Car();
  void renderCar();
  bool updateCar();
  glm::vec2 getLocation();
  bool doCheck();
};

}

#endif
