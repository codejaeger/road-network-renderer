#include "road_network/Traffic-Manager/traffic_light.hpp"

namespace soc {

Light::Light(Vertex &vertex) {
  v = vertex;
  size = v.outgoing.size();
  flag = 0;
}

void Light::updateLight() {
  flag++;
  if (flag >= size) {
    flag = 0;
  }
  std::cout << "Updated Light to " << flag << std::endl;
}

void Light::renderLight() {
  std::cout << "renderLight\n";
}

unsigned int Light::returnEdgeNumber() {
  return (unsigned int)v.outgoing[flag];
}

}
