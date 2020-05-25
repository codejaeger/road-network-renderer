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
  Graph *g;
  std::vector<glm::vec2> path;
  glm::vec2 current;
  glm::vec2 tangent;
public:
 void renderCar();
 void updateCar();
 Car(Graph* graph, std::vector<int> in);
};

}

#endif
