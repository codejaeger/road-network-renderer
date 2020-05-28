#include "road_network/Traffic-Manager/intersection_lights.hpp"

namespace soc {

IntersectionLights::IntersectionLights(Graph *graph, unsigned int index) {
  Vertex vertex = graph->v[index];
  center = vertex.origin;

  for (unsigned int i = 0; i < vertex.outgoing.size(); i++) {
    if (int(graph->e[vertex.outgoing[i]].path.size()) > 1)
      edge_firsts.push_back(graph->e[vertex.outgoing[i]].path[1]);
  }

  size = edge_firsts.size();

  float x1 = center[0];
  float y1 = center[1];

  float road_depth = 0.02;
  float lane_width = 0.05;

  for (unsigned int i = 0; i < size; i++) {
    mod.push_back(new TrafficLightModel(0.04));

    float x2 = edge_firsts[i][0];
    float y2 = edge_firsts[i][1];
    glm::vec2 tangent = glm::vec2((x2-x1)/2, (y2-y1)/2);
    glm::vec2 normal = glm::vec2((y2-y1), (x1-x2));
    std::cout << tangent[0] << "," << tangent[1] << "..."
              << normal[0] << "," << normal[1] << " Traffic Light Model Updated\n";

    glm::vec2 loc = edge_firsts[i] - tangent;

    float rz;
    if (normal[0] > 0.0) {
      rz = 180 + atan(normal[1]/normal[0])*180.0/PI_MATH;
    }
    else if (normal[0] == 0.0) {
      rz = 90.0;
    }
    else {
      rz = (atan(normal[1]/normal[0])*180.0/PI_MATH);
    }
    mod[i]->change_parameters(loc[0], loc[1], 0, 0, 0, rz);
  }
  flag = 0;
  mod[flag]->turnGreen();

}

void IntersectionLights::updateLight() {
  mod[flag]->turnRed();
  flag++;
  if (flag >= size) {
    flag = 0;
  }
  mod[flag]->turnGreen();
  // std::cout << "Updated IntersectionLights to " << flag << std::endl;
}

void IntersectionLights::renderLight() {
  // std::cout << "renderLight\n";
  for (unsigned int i = 0; i < size; i++) {
    mod[i]->render();
  }
}

glm::vec2 IntersectionLights::returnEdgeNumber() {
  return edge_firsts[flag];
}

IntersectionLights::~IntersectionLights() {
  std::cout << "IntersectionLights Deleted\n";
  for (unsigned int i = 0; i < mod.size(); i++) {
    delete mod[i];
  }
}

}
