#ifndef car_model_hpp
#define car_model_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Graph/graph.hpp"
#include "road_network/Models/car_model.hpp"

#define PI 3.14159

namespace soc {

class CarNode	{
public:
  Graph *g;
  CarModel *mod;
  std::vector<glm::vec2> path;
  unsigned int current;
  std::vector<glm::vec2> check_loc;
  glm::vec2 tangent;
  glm::vec2 normal;
  CarNode(Graph* graph, std::vector<int> in);
  ~CarNode();
  void renderCar();
  bool updateCar();
  glm::vec2 getLocation();
  glm::vec2 getNextLocation();
  bool doCheck();
};

}

#endif
