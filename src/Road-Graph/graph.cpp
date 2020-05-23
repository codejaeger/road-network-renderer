#include "road_network/Road-Graph/graph.hpp"

namespace soc {
	Graph::Graph(float hw): HALF_WIDTH(hw){}

  void Graph::addVertex(glm::vec2 origin) {
	  Vertex vert;
	  vert.origin = origin;
	  v.push_back(vert);
	  vertices.resize(v.size());
  }

  void Graph::addEdge(std::vector<glm::vec2> path) 
  { 
		Edge edge;
		edge.from = -1;
		edge.to = -1;
		int start_match=-1;
		int end_match=-1;
		bool a=false, b = false;
		// check if the path has one endpoint as one of the vertices
		for(int i=0; i<v.size(); i++){
			if(calc_dist(path[0], v[i].origin)==0){
				edge.from = i;
				start_match = i;
				a = true;
				break;
			}
		}
		for(int i=0; i<v.size(); i++){
			if(calc_dist(path[path.size()-1], v[i].origin)==0){
				edge.to = i;;
				end_match = i;
				b = true;
				break;
			}
		}
		// if the start point is not any of the vertices then check it lies inside which intersection
		if(edge.from == -1)
			for(int i=0; i<v.size(); i++){
				if(calc_dist(path[0], v[i].origin)<4*HALF_WIDTH){
					// check if the endpoint is not that origin in case endpoint is a vertex and it is closer than 4d to it
					if(path[path.size()-1]!=v[i].origin) {
						edge.from = i;
						start_match = i;
						break;
					}
				}   
			}
		// if the end point is not any of the vertices then check it lies inside which intersection
		if(edge.to == -1)
			for(int i=0; i<v.size(); i++){
				// check if the startpoint is not that origin in case startpoint is a vertex and it is closer than 4d to it
				if(calc_dist(path[path.size()-1], v[i].origin)<4*HALF_WIDTH){
					if(path[0]!=v[i].origin) {
						edge.to = i;
						end_match = i;
						break;
					}
				}
			}
		// if both of the endpoints are part of intersections
		if(!(a || b)) {
			edge.path = path;
			float dist;
			for(int i=0; i<path.size()-1; i++){
				dist += calc_dist(path[i], path[i+1]);
			}
			edge.distance = dist;
		}
		// if only endpoints is a part of the intersection 
		else if(a) {
			path.erase(path.begin());
			edge.path = path;
			float dist;
			for(int i=0; i<path.size()-1; i++){
				dist += calc_dist(path[i], path[i+1]);
			}
			edge.distance = dist; 
		}
		// if only start point is a part of the intersection
		else if(b) {
			path.erase(path.begin() + path.size() -1);
			edge.path = path;
			float dist;
			for(int i=0; i<path.size()-1; i++){
				dist += calc_dist(path[i], path[i+1]);
			}
			edge.distance = dist;
		}
		// if both are not a part of intersection
		else {
			path.erase(path.begin() + path.size() -1);
			path.erase(path.begin());
			edge.path = path;
			float dist;
			for(int i=0; i<path.size()-1; i++){
				dist += calc_dist(path[i], path[i+1]);
			}
			edge.distance = dist;
		}
		e.push_back(edge);
		v[start_match].outgoing.push_back(e.size()-1);
		vertices[start_match].push_back(end_match);
		// now the same things for storing the reverse path since the edge is directed
		Edge edge_rev;
		edge_rev.distance = edge.distance;
		edge_rev.from = edge.to;
		edge_rev.to = edge.from;
		edge_rev.path = edge.path; 
		std::reverse(edge_rev.path.begin(), edge_rev.path.end());
		e.push_back(edge_rev);
		v[end_match].outgoing.push_back(e.size() - 1);
		vertices[end_match].push_back(start_match);
  } 
	
	float Graph::calc_dist(glm::vec2 a, glm::vec2 b) {
		return sqrt((b[0]-a[0])*(b[0]-a[0]) + (b[1]-a[1])*(b[1]-a[1]));
	}

	// for explanation refer to djikstra algorithm of finding shortest path
	std::vector<int> Graph::dijkstra(std::vector< std::vector<float> > G, int startnode, int endnode) {
		int n = G.size();
	  float cost[n][n],distance[n],pred[n];
	  int visited[n],count,mindistance,nextnode,i,j;
	  for(i=0;i<n;i++)
	     for(j=0;j<n;j++)
		if(G[i][j]==0)
	    cost[i][j]=INFINITY;
		else
	    cost[i][j]=G[i][j];
	  for(i=0;i<n;i++) {
	    distance[i]=cost[startnode][i];
	    pred[i]=startnode;
	  	visited[i]=0;
	  }
	  distance[startnode]=0;
		visited[startnode]=1;
	  count=1;
	  while(count<n-1) {
      mindistance=INFINITY;
      for(i=0;i<n;i++)
        if(distance[i]<mindistance && !visited[i]) {
        	mindistance=distance[i];
        	nextnode=i;
	  		}
      visited[nextnode]=1;
      for(i=0;i<n;i++)
        if(!visited[i])
      if(mindistance+cost[nextnode][i]<distance[i]) {
        distance[i]=mindistance+cost[nextnode][i];
        pred[i]=nextnode;
      }
	    count++;
		}
		std::vector<int> nodes; 
	  if(endnode!=startnode) {
      nodes.push_back(endnode);
      j=endnode;
	    do {
	      j=pred[j];
	 	    nodes.push_back(j);
	    }while(j!=startnode);
	  }

	  return nodes;
	}
	std::vector<int> Graph::getPath(int startnode, int endnode) {
		std::vector< std::vector<float> > G(v.size());
		// initialize the matrix to feed to the djikstra function where ijth element represent the distance between ith and jth node if they are neighbours
		// and it is 0 if they are not connected directly
		for(int i=0; i<v.size(); i++) {
			for(int j=0; j<v.size(); j++) {
				float dist=0;
				for(int k=0; k<v[i].outgoing.size(); k++) {
					if(e[v[i].outgoing[k]].to == j)
						dist = e[v[i].outgoing[k]].distance;
				}
				G[i].push_back(dist);
			}
		}
		std::vector<int> path = dijkstra(G, startnode, endnode);
		// the returned path is opposite to what we expect so reverse it
		std::reverse(path.begin(), path.end());
		// add the edges indices also in between the vertices
		for(int i=path.size()-1; i>0; i--) {
			for(int j=0; j<v[path[i-1]].outgoing.size(); j++) {
					if(e[v[path[i-1]].outgoing[j]].to == path[i])
						path.insert(path.begin() + i, v[path[i-1]].outgoing[j]);
			}
		}

		return path;
	} 
}