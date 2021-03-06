#ifndef intersection_lights_hpp
#define intersection_lights_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Graph/graph.hpp"
#include "road_network/Models/traffic_light_model.hpp"

#include "road_network/constants.hpp"

namespace soc {

class IntersectionLights {
public:
  glm::vec2 center;
  std::vector<glm::vec2> edge_seconds;
  std::vector<TrafficLightModel*> mod;
  unsigned int size;
  unsigned int flag;
  bool is_green;
  IntersectionLights(Graph *graph, unsigned int index);
  ~IntersectionLights();
  void updateLightGreen();
  void updateLightYellow();
  void renderLight();
  glm::vec2 returnGoPoint();
};

} // End namespace soc

#endif /* intersection_lights_hpp */
