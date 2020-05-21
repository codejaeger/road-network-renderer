#include "road_network/Road-Model/intersection.hpp"

namespace soc {
	// initialize the data-members to 0
	Intersection::Intersection(){
		x=0;
		intersection_depth=0;
	}

	// inittialize the properties of the intersection class
	void Intersection::initIntersection(float x, float depth, glm::vec2 origin, std::vector<int> type1, std::vector<int> type2, std::vector<glm::vec2> endpoints, bool init_roads){
		this->x = x;
		this->intersection_depth = depth;
		this->origin = origin;
		this->type1 = type1;
		this->type2 = type2;
		this->endpoints = endpoints;
		k = 0;   //set the radius of the inner-circle to be 0
		if(init_roads) {
			// interpolate points between the edge between two roads to give it a curved shape
			fill_interpolate_points();
			// lay the roads in the intersection region
			initRoadObjects();
		}

	}

	// return the point on bezier curve corresponding to the ratio 
	std::vector<glm::vec2> Intersection::bezier_curve_point(std::vector<glm::vec2> pos,float ratio) {
	  // For logic goto
	  //   https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Higher-order_curves
	  if (pos.size() == 1)
	    return pos;

	  std::vector<glm::vec2> new_pos;
	  glm::vec2 temp;
	  for (int i = 0; i < (pos.size() - 1); i++) {
	    temp = glm::vec2((pos[i][0] * (1 - ratio)) + (pos[i + 1][0] * ratio),
	                     (pos[i][1] * (1 - ratio)) + (pos[i + 1][1] * ratio));
	    new_pos.push_back(temp);
	  }

	  return bezier_curve_point(new_pos, ratio);
	}

	// fill the interpolate in the edge between two roads
	// here we assume the endpoints are already sorted in acw sense
	void Intersection::fill_interpolate_points() {
		for (int i = 0; i < endpoints.size()-1 ; i+=2){
			//midpoint is a point on the angle bisector of the consecutive endpoints at a distance of 1.5x from the origin
			glm::vec2 v = normalize(normalize(endpoints[i+1]-origin) + normalize(endpoints[(i+2)%endpoints.size()] - origin));
			glm::vec2 midpoint(1.5*x*v[0] + origin[0], 1.5*x*v[1]+origin[1]);
			std::vector<glm::vec2> temp(3);
			temp[0] = endpoints[i+1];
			temp[1] = midpoint;
			temp[2] = endpoints[(i+2)%endpoints.size()];
			std::vector<glm::vec2> positions;
			for (float j = 0; j <= BZC_COUNT; j++) {
		      std::vector<glm::vec2> pos = bezier_curve_point( temp , (j / BZC_COUNT));
		      positions.push_back(pos[0]);
		   }
		   interpolate_points.push_back(positions);
		}
	}

	void Intersection::initRoadObjects() {
		// initialize roads between road entry and the center of the intersection
		for(int i=0; i<interpolate_points.size(); i++){
			for(int j=0; j<20; j++){
				glm::vec2 a1 = int_div(endpoints[2*i], endpoints[2*i +1], j/(20-j));
				glm::vec2 a2;
				if(j==19)
					a2 = endpoints[2*i + 1];
				else
					a2 = int_div(endpoints[2*i], endpoints[2*i +1], j+1/(19-j));
				glm::vec2 v1 = k*normalize(a1-origin) + origin;
				glm::vec2 v2 = k*normalize(a2-origin) + origin;
				Road road_temp(a1, v1, v2, a2 ,intersection_depth, 0, 0, false, false);
				r.push_back(road_temp);
			}
			//intialize roads between interpolated points and center of the intersection
			float a=0, b=0;
			for(int j=0; j<interpolate_points[i].size()-1; j++){
				glm::vec2 v1 = k*normalize(interpolate_points[i][j]-origin) + origin;
				glm::vec2 v2 = k*normalize(interpolate_points[i][j+1]-origin) + origin;
				Road road_temp(interpolate_points[i][j], v1, v2, interpolate_points[i][j+1], intersection_depth, b, a, true, false);
				b = road_temp.end_spacing_b;
				a = road_temp.end_spacing_a;
				r.push_back(road_temp);
			}
		}
	}

	// render the constituent roads of the intersection
	void Intersection::render(){
		for(int i=0; i<r.size(); i++){
			r[i].render();
		}
	}

	// returns the distance between two points in 2d
	float Intersection::calc_dist(glm::vec2 v1, glm::vec2 v2) {
	  return sqrt((v1[0] - v2[0]) * (v1[0] - v2[0]) + (v1[1] - v2[1]) * (v1[1] - v2[1]));
	}

	// returns the point dividing the two given points in the ratio r internally
	glm::vec2 Intersection::int_div(glm::vec2 v1, glm::vec2 v2, float r) {
		float a, b;
	  a = (r * v2[0] + v1[0]) / (1 + r);
	  b = (r * v2[1] + v1[1]) / (1 + r);
	  return glm::vec2(a, b);
	}

	// returns the unit vector along the input vector
	glm::vec2 Intersection::normalize(glm::vec2 v) {
	  glm::vec2 n;
	  GLfloat f = sqrt(v[0] * v[0] + v[1] * v[1]);
	  n[0] = v[0] / f;
	  n[1] = v[1] / f;
	  return n;
	}
}
