#ifndef road_collection_hpp
#define road_collection_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>
#include <vector>
#include <stdlib.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "road_network/Road-Graph/graph.hpp"
#include "road_network/constants.hpp"

#include "road_network/Road-Model/road.hpp"
#include "road_network/Road-Model/road_sep.hpp"
#include "road_network/Road-Model/intersection.hpp"

namespace soc {

class RoadNetwork {
  float road_depth;
  float d;  // half of the width of the road
  unsigned int num_road_networks;  // number of roads in the network
  std::vector< std::vector<Road> > r;    // stores the road objects
  std::vector< std::vector<RoadSep> > rs;  // stores the road sep objects
  std::vector< std::vector<glm::vec2> > bezier_positions; // bezier_positions stores the interpolated points after reading from raw file
  std::vector< std::vector<glm::vec2> > tangent_directions; //tangents along the beizer-curve
  std::vector< std::vector<glm::vec2> > road_corners; //points on either side of curve at a fixed distance othogonal to it
  std::string file_name;
  std::vector<Intersection> intersection;
  Graph g;
  void fill_road_corners();
  void fill_tangent_directions();
  glm::vec2 normalize(glm::vec2 v);
  void initRoads();
  void initRoadSeps();
  void initIntersections();
  void mergeCloseIntersections();
  void deleteRoadsInsideIntersection();
  void deleteRoadSepsInsideIntersection();
  void findIntersections(int m, int n);
  void fill_endpoints_type(int m, int n, glm::vec2 origin, int i, int j, std::vector<int> &type1, std::vector<int> &type2, std::vector<glm::vec2> &endpoints);
  void sort_endpoints(std::vector<glm::vec2> &endpoints);
  void sort_pair_endpoints(std::vector<glm::vec2> &endpoints, glm::vec2 origin);
  bool isACW(glm::vec2 v1, glm::vec2 v2, glm::vec2 origin);
  bool doIntersect(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2);
  glm::vec2 findOrigin(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2);
  void swap(glm::vec2 &v1, glm::vec2 &v2);
  void saveIntersections();
  void readIntersections();
  void initGraph();
public:
  RoadNetwork(float rd, float half_width, std::string file_name);
  ~RoadNetwork();
  void initRoadNetwork();
  void renderRoads();
  void renderRoadSeps();
  void renderIntersections();
  Graph* getGraph();
  float static calc_dist(glm::vec2 v1, glm::vec2 v2);
  int static orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r);
};

} // End namespace soc

#endif /* road_collection_hpp */
