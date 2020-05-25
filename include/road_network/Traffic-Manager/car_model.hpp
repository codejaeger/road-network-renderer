#ifndef car_model_hpp
#define car_model_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Model/road_collection.hpp"

namespace soc {

class Car	{
public:
  Graph *g;
  std::vector<glm::vec2> path;
  unsigned int current;
  glm::vec2 tangent;
  glm::vec2 normal;
  Car(Graph* graph, std::vector<int> in);
  ~Car();
  void renderCar();
  bool updateCar();
};

}

#endif
