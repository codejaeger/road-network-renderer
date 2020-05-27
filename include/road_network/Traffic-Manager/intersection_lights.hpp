#ifndef intersection_lights_hpp
#define intersection_lights_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Graph/graph.hpp"
#include "road_network/Models/traffic_light_model.hpp"

#define PI_MATH 3.14159265

namespace soc {

class IntersectionLights {
public:
  glm::vec2 center;
  std::vector<glm::vec2> edge_firsts;
  std::vector<TrafficLightModel*> mod;
  unsigned int size;
  unsigned int flag;
  IntersectionLights(Graph *graph, unsigned int index);
  ~IntersectionLights();
  void updateLight();
  void renderLight();
  glm::vec2 returnEdgeNumber();
};

}

#endif /* intersection_lights_hpp */
