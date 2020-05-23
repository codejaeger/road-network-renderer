#ifndef graph_hpp
#define graph_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <glm/vec2.hpp>
#include <math.h>
#include <algorithm>
namespace soc {
	struct Vertex;   // this is a forward reference 

	struct Edge{
    std::vector<glm::vec2> path;
    float distance;
    int from, to;  // stores the indices of the from and to vertex of this directed edge
	};

	struct Vertex{
    glm::vec2 origin;
    std::vector<int> outgoing;  //stores the indices of the outgoing edges
	};

	class Graph 
	{
	public:
		const float HALF_WIDTH;
    std::vector<Vertex> v;   // stores the vertices of the graph
    std::vector<Edge> e;     // stores the edges of the graph
    std::vector< std::vector<int> > vertices;   // stores the neighbouring vertices indices of all the vertices
	  float calc_dist(glm::vec2 a, glm::vec2 b); 
		Graph(float hw);
    void addVertex(glm::vec2 origin);
    void addEdge(std::vector<glm::vec2> path);
    std::vector<int> dijkstra(std::vector< std::vector<float> > G, int startnode, int endnode);
    std::vector<int> getPath(int startnode, int endnode);
	};

}
#endif

