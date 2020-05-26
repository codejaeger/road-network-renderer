#include "road_network/Traffic-Manager/intersection_lights.hpp"

namespace soc {

IntersectionLights::IntersectionLights(Vertex &vertex) {
  v = vertex;
  size = v.outgoing.size();
  flag = 0;
}

void IntersectionLights::updateLight() {
  flag++;
  if (flag >= size) {
    flag = 0;
  }
  std::cout << "Updated IntersectionLights to " << flag << std::endl;
}

void IntersectionLights::renderLight() {
  std::cout << "renderLight\n";
}

unsigned int IntersectionLights::returnEdgeNumber() {
  return (unsigned int)v.outgoing[flag];
}

}
