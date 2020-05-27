#ifndef intersection_hpp
#define intersection_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "road_network/shader_util.hpp"

#include "road_network/Road-Model/road.hpp"

#define BZC_COUNT 20

namespace soc {

class Intersection{
	float x; //2*x is the radius of circle inside which roads are not rendered, x = 2 * width_road
	float k; //the radius of circle inside the intersection, k = half_width of road
	float intersection_depth;
	//for rendering intersection
	std::vector<Road> r;
	std::vector<glm::vec2> bezier_curve_point(std::vector<glm::vec2> pos,float ratio);
	void fill_interpolate_points();
	void initRoadObjects();
	float calc_dist(glm::vec2 v1, glm::vec2 v2);
	glm::vec2 int_div(glm::vec2 v1, glm::vec2 v2, float r);
	glm::vec2 normalize(glm::vec2 v);
public:
	glm::vec2 origin;   //stores the center of the intersection
	std::vector<int> type1;  //type 1 are those types of roads which pass through intersection completely
	std::vector<int> type2; //it stores the indices of roads which either originate or end at the intersection point
	std::vector<glm::vec2> endpoints;  //stores the endpoints of the incoming roads in anticlockwise manner
	std::vector< std::vector<glm::vec2> > interpolate_points;
	Intersection();
	void initIntersection(float x, float depth, glm::vec2 origin, std::vector<int> type1, std::vector<int> type2, std::vector<glm::vec2> endpoints, bool init_roads = true);
	void render();
};

}

#endif
