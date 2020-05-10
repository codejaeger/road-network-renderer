#ifndef road_collection_hpp
#define road_collection_hpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "road_network/Road-Model/road.hpp"
#include "road_network/Road-Model/road_sep.hpp"

namespace soc {

class RoadNetwork {
  float road_depth;
  // half of the width of the road
  float d;
  int num_road_networks;
  // stores the road objects
  std::vector< std::vector<Road> > r;
  // stores the road sep objects
  std::vector< std::vector<RoadSep> > rs;
  std::vector< std::vector<glm::vec2> > bezier_positions;
  // bezier_positions stores the interpolated points after reading from raw file
  std::vector< std::vector<glm::vec2> > tangent_directions;
  std::vector< std::vector<glm::vec2> > road_corners;
  std::string file_name;

  void fill_road_corners();
  void fill_tangent_directions();
  glm::vec2 normalize(glm::vec2 v);
  void initRoads();
  void initRoadSeps();

public:
  RoadNetwork(float rd, float half_width, std::string file_name);
  ~RoadNetwork();
  void initRoadNetwork();
  void renderRoads();
  void renderRoadSeps();
};

} // End namespace soc

#endif /* road_collection_hpp */
