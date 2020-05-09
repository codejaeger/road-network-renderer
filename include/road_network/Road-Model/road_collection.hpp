#ifndef road_collection_hpp
#define road_collection_hpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "road_network/Road-Model/road.hpp"
#include "road_network/Road-Model/road_sep.hpp"

namespace soc
{
	class RoadNetwork
	{	
		float road_depth;     
		float d;              //half of the width of the road
		int num_road_networks;
		std::vector< std::vector<Road> > r;    //stores the road objects
		std::vector< std::vector<RoadSep> > rs; //stores the road sep objects
		std::vector< std::vector<glm::vec2> > bezier_positions;  //stores the interpolated points after reading from raw file
		std::vector< std::vector<glm::vec2> > tangent_directions;  
		std::vector< std::vector<glm::vec2> > road_corners;

		void fill_road_corners();
		void fill_tangent_directions();
		glm::vec2 normalize(glm::vec2 v);
		void initRoads();
		void initRoadSeps();
	public:
		RoadNetwork(float rd, float half_width);
		~RoadNetwork();
		void initRoadNetwork(std::string s);
		void renderRoads();
		void renderRoadSeps();

	};
}

#endif