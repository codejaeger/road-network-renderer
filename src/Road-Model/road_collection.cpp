#include "road_network/Road-Model/road_collection.hpp"
extern double PI;
namespace soc
{
	//initialize the road-depth and road-width vaiable
	RoadNetwork::RoadNetwork(float rd, float half_width)
	{
		road_depth = rd;
		d = half_width;
		num_road_networks = 0;
	}
	//fills the bezier_positions after reading from external raw file
	void RoadNetwork::initRoadNetwork(std::string s)
	{	r.clear();
		rs.clear();
		bezier_positions.clear();
		tangent_directions.clear();
		road_corners.clear();

		std::ifstream myData(s);   
		if(!myData.good()){
			std::cerr<<"Can't read the raw file"<<std::cout;
		}                                        
		int i = 0, j =0;
		char buf[sizeof(glm::vec2)];
		while (myData.read(buf,sizeof(buf)))
		{ 
			bezier_positions.resize(j+1);
			glm::vec2 v;
	    	memcpy(&v, buf, sizeof(glm::vec2));
	    	bezier_positions[j].push_back(v);
	    	i++;
	    	j=i/51;
		}
		num_road_networks=j+1;
		//initialize the Road and RoadSep object vectors using the values stores in bezier-curve positions 
		fill_tangent_directions();
		fill_road_corners();
		initRoads();
		initRoadSeps();
	}
	//fills the directions of tangent to each point in the road..which helps in aligning the road-separators
	void RoadNetwork::fill_tangent_directions()
	{
		tangent_directions.resize(bezier_positions.size());
		for(int i =0; i<bezier_positions.size(); i++)
		{
			for(int j =0; j < bezier_positions[i].size(); j++)
			{
				if(j==0)
					tangent_directions[i].push_back(normalize(bezier_positions[i][j+1] - bezier_positions[i][j]));
				else if(j == (bezier_positions[i].size()-1))
					tangent_directions[i].push_back(normalize(bezier_positions[i][j] - bezier_positions[i][j-1]));
				else 
					tangent_directions[i].push_back(normalize(bezier_positions[i][j+1] - bezier_positions[i][j-1]));
			}
		}
	}
	//generates points on either side of the bezier curve in accordance with the width of the road
	void RoadNetwork::fill_road_corners()
	{
		road_corners.resize(2*bezier_positions.size());
		std::vector< std::vector<glm::vec2> > n_up(bezier_positions.size()), n_down(bezier_positions.size());
		for(int i =0; i<bezier_positions.size(); i++)
		{
			for(int j=0; j<bezier_positions[i].size(); j++)
			{
				n_up[i].push_back(glm::vec2(-tangent_directions[i][j][1], tangent_directions[i][j][0]));
				n_down[i].push_back(glm::vec2(tangent_directions[i][j][1], -tangent_directions[i][j][0]));
			}
		}
		for(int i =0; i<bezier_positions.size() ; i++)
			for(int j=0; j<(2*bezier_positions[i].size()-1); j+=2)
			{
				road_corners[i].push_back(bezier_positions[i][j/2] + d * n_down[i][j/2]);
				road_corners[i].push_back(bezier_positions[i][j/2] + d * n_up[i][j/2]);
			}
	}
	//initializes the road objects using the road-corners generated before
	void RoadNetwork::initRoads()
	{
		r.resize(bezier_positions.size());
		for(int i =0; i<bezier_positions.size(); i++)
		{
			float b=0, a=0;
			for(int j=0; j<road_corners[i].size()-2; j+=2)
			{
				r[i].push_back(Road(road_corners[i][j], road_corners[i][j+1], road_corners[i][j+3], road_corners[i][j+2], road_depth/2, 0, 0));
				b = r[i][j/2].end_spacing_b;
				a = r[i][j/2].end_spacing_a;
			}
		}
	}
	//initializes the RoadSep at the bezier-points at a fixed interval of points along the tangent-directions
	void RoadNetwork::initRoadSeps()
	{
		rs.resize(bezier_positions.size());
		for(int i= 0 ; i<bezier_positions.size(); i++)
		{
			for(int j=5; j<bezier_positions[i].size()-5; j+=8)
			{
				RoadSep rs_temp(glm::vec3(0, 0, 0), 0.02);
				rs_temp.change_parameters(bezier_positions[i][j][0], bezier_positions[i][j][1], road_depth/2, 0, 0, 180/PI * atan(tangent_directions[i][j][1]/tangent_directions[i][j][0]));
				rs[i].push_back(rs_temp);			
			}
		}
	}
	//returns the unit vector along the input vector
	glm::vec2 RoadNetwork::normalize(glm::vec2 v)
	{
		glm::vec2 n;
		GLfloat f = sqrt(v[0]*v[0] + v[1]*v[1]);
		n[0] = v[0]/f;
		n[1] = v[1]/f;
		return n;
	}
	//renders the Roads on the screen
	void RoadNetwork::renderRoads()
	{
		for(int i =0; i<r.size(); i++)
		{
			for(int j=0; j<r[i].size(); j++)
			{
				r[i][j].render();
			}
		}
	}
	//renders the Roadseps on the screen
	void RoadNetwork::renderRoadSeps()
	{
		for(int i =0; i<rs.size(); i++)
		{
			for(int j=0; j<rs[i].size(); j++)
			{
				rs[i][j].render();
			}
		}
	}
	RoadNetwork::~RoadNetwork(){
		
	}
}