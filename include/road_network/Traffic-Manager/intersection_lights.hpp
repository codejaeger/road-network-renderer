#ifndef traffic_light_hpp
#define traffic_light_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>

#include "road_network/Road-Graph/graph.hpp"

namespace soc {

class IntersectionLights {
public:
  Vertex v;
  unsigned int size;
  unsigned int flag;
  IntersectionLights(Vertex &vertex);
  ~IntersectionLights();
  void updateLight();
  void renderLight();
  unsigned int returnEdgeNumber();
};

}

#endif