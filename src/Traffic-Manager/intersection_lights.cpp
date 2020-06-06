#include "road_network/Traffic-Manager/intersection_lights.hpp"

namespace soc {

IntersectionLights::IntersectionLights(Graph *graph, unsigned int index) {
  // Get the vertex
  Vertex vertex = graph->v[index];
  center = vertex.origin;

  // Save all the locations of intersection's stop points
  for (unsigned int i = 0; i < vertex.outgoing.size(); i++) {
    if (int(graph->e[vertex.outgoing[i]].path.size()) > 1)
      edge_seconds.push_back(graph->e[vertex.outgoing[i]].path[1]);
  }

  size = edge_seconds.size();

  float x1 = center[0];
  float y1 = center[1];

  float road_depth = 0.02;
  float lane_width = 0.05;

  // For each incoming road, assign a traffic light
  for (unsigned int i = 0; i < size; i++) {
    // Assign them a model
    mod.push_back(new TrafficLightModel(0.04));

    float x2 = edge_seconds[i][0];
    float y2 = edge_seconds[i][1];
    glm::vec2 tangent = glm::vec2((x2-x1)/2, (y2-y1)/2);
    glm::vec2 normal = glm::vec2((y2-y1), (x1-x2));
    std::cout << tangent[0] << "," << tangent[1] << "..."
              << normal[0] << "," << normal[1] << " Traffic Light Model Updated\n";

    glm::vec2 loc = edge_seconds[i] - tangent;

    float rz;
    if (normal[0] > 0.0) {
      rz = 180 + atan(normal[1]/normal[0])*180.0/PI;
    }
    else if (normal[0] == 0.0) {
      rz = 90.0;
    }
    else {
      rz = (atan(normal[1]/normal[0])*180.0/PI);
    }
    mod[i]->change_parameters(loc[0], loc[1], 0, 0, 0, rz);
  }

  // The first light is set to green.
  flag = 0;
  is_green = true;
  mod[flag]->turnGreen();
}

void IntersectionLights::updateLightGreen() {
  // The current yellow light is turned red and the next one is turned green
  is_green = true;
  mod[flag]->turnRed();
  flag++;
  if (flag >= size) {
    flag = 0;
  }
  mod[flag]->turnGreen();
  // std::cout << "Updated IntersectionLights to " << flag << std::endl;
}

void IntersectionLights::updateLightYellow() {
  // The current green light is turned to yellow
  is_green = false;
  mod[flag]->turnYellow();
}

void IntersectionLights::renderLight() {
  // Render all the traffic light models
  // std::cout << "renderLight\n";
  for (unsigned int i = 0; i < size; i++) {
    mod[i]->render();
  }
}

glm::vec2 IntersectionLights::returnGoPoint() {
  // Return the intersection stop points' location.
  return edge_seconds[flag];
}

IntersectionLights::~IntersectionLights() {
  // destructor
  std::cout << "IntersectionLights Deleted\n";
  for (unsigned int i = 0; i < mod.size(); i++) {
    delete mod[i];
  }
}

} // End namespace soc
