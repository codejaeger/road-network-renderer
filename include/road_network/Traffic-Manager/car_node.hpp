#ifndef car_node_hpp
#define car_node_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Graph/graph.hpp"
#include "road_network/Models/car_model.hpp"

#include "road_network/constants.hpp"

namespace soc {

class CarNode	{
public:
  CarModel *mod;
  int start_vertex_no;
  std::vector<glm::vec2> path, path_centered;
  unsigned int current;
  std::vector<glm::vec2> check_loc, check_loc_centered;
  float lane_width;
  float road_depth;
  CarNode(Graph* graph, std::vector<int> in);
  ~CarNode();
  void renderCar();
  bool updateCar();
  glm::vec2 getLocation();
  glm::vec2 getLocationCentered();
  std::vector<glm::vec2> getCollisionLocations();
  bool doCheck();
  void assignLane();
  void bezierCurve();
};

}

#endif /* car_node_hpp */
