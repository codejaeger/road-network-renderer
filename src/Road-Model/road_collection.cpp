#include "road_network/Road-Model/road_collection.hpp"

extern double PI;

namespace soc {

glm::vec2 p0;
// initialize the road-depth, road-width variable and the name of the raw file containing bezier-points
RoadNetwork::RoadNetwork(float rd, float half_width, std::string file): g(half_width) {
  road_depth = rd;
  d = half_width;
  num_road_networks = 0;
  file_name = "./models/Bezier-Model/" + file;
}

// fills the bezier_positions after reading from external raw file
void RoadNetwork::initRoadNetwork() {
  //clear out all  the vectors in case of reinitialization of road-network
  r.clear();
  rs.clear();
  bezier_positions.clear();
  tangent_directions.clear();
  road_corners.clear();
  intersection.clear();

  //reading in points from external raw file according to the format in which they were stored
  std::fstream fp;
  fp.open(file_name.c_str(), std::ios::binary | std::ios::in);
  if (!fp.good()) {
    std::cout << "could not read from the raw file" << std::endl;
    return;
  }
  glm::vec2 total_paths;
  fp.read((char *)&total_paths, sizeof(total_paths));
  num_road_networks = total_paths[0];
  for (int i = 0; i < int(total_paths[0]); i++) {
    bezier_positions.resize(i + 1);
    glm::vec2 num;
    fp.read((char *)&num, sizeof(num));
    glm::vec2 ip[int(num[0])];
    fp.read((char *)&ip, sizeof(ip));
    for (int j = 0; j < int(num[0]); j++) {
      bezier_positions[i].push_back(ip[j]);
    }
  }
  fp.close();

  /* initialize the Road and RoadSep object vector 
  using the values stores in bezier-curve positions*/
  fill_tangent_directions();
  fill_road_corners();
  initRoads();
  initRoadSeps();
  // detect intersections of curves and initialize intersections at those junctionss
  initIntersections();
  /* this is to merge very close intersections into a big one 
  with all the connections from the constituent ones */
  mergeCloseIntersections();
  //delete the roads and road-seps from the region where intersection was created
  saveIntersections();
  initGraph();
  deleteRoadsInsideIntersection();
  deleteRoadSepsInsideIntersection();
}

/* fills the directions of tangent to each point in the road
 which helps in aligning the road-separators */
void RoadNetwork::fill_tangent_directions() {
  tangent_directions.resize(bezier_positions.size());
  for (int i = 0; i < bezier_positions.size(); i++) {
    for (int j = 0; j < bezier_positions[i].size(); j++) {
      if (j == 0)
        tangent_directions[i].push_back(
            normalize(bezier_positions[i][j + 1] - bezier_positions[i][j]));
      else if (j == (bezier_positions[i].size() - 1))
        tangent_directions[i].push_back(
            normalize(bezier_positions[i][j] - bezier_positions[i][j - 1]));
      else
        tangent_directions[i].push_back(
            normalize(bezier_positions[i][j + 1] - bezier_positions[i][j - 1]));
    }
  }
}

/* generates points at a distance of half-width 
   on either side of the bezier curve
   orthogonal to it */
void RoadNetwork::fill_road_corners() {
  road_corners.resize(2 * bezier_positions.size());
  std::vector< std::vector<glm::vec2> > n_up(bezier_positions.size()),
      n_down(bezier_positions.size());
  for (int i = 0; i < bezier_positions.size(); i++) {
    for (int j = 0; j < bezier_positions[i].size(); j++) {
      n_up[i].push_back(
          glm::vec2(-tangent_directions[i][j][1], tangent_directions[i][j][0]));
      n_down[i].push_back(
          glm::vec2(tangent_directions[i][j][1], -tangent_directions[i][j][0]));
    }
  }
  for (int i = 0; i < bezier_positions.size(); i++) {
    for (int j = 0; j < (2 * bezier_positions[i].size() - 1); j += 2) {
      road_corners[i].push_back(bezier_positions[i][j / 2] +
                                d * n_down[i][j / 2]);
      road_corners[i].push_back(bezier_positions[i][j / 2] +
                                d * n_up[i][j / 2]);
    }
  }
}

// initializes the road objects using the road-corners generated before
void RoadNetwork::initRoads() {
  r.resize(bezier_positions.size());
  for (int i = 0; i < bezier_positions.size(); i++) {
    float b = 0, a = 0;
    for (int j = 0; j < road_corners[i].size() - 2; j += 2) {
      r[i].push_back(Road(road_corners[i][j], road_corners[i][j + 1],
                          road_corners[i][j + 3], road_corners[i][j + 2],
                          road_depth, b, a));
      b = r[i][j / 2].end_spacing_b;
      a = r[i][j / 2].end_spacing_a;
    }
  }
}

// initializes the RoadSep at the bezier-points at a fixed interval of points
// along the tangent-directions
void RoadNetwork::initRoadSeps() {
  rs.resize(bezier_positions.size());
  for (int i = 0; i < bezier_positions.size(); i++) {
    for (int j = 5; j < bezier_positions[i].size() - 5; j += 15) {
      RoadSep rs_temp(glm::vec3(0, 0, 0), (d/10));
      rs_temp.change_parameters(bezier_positions[i][j][0],
                                bezier_positions[i][j][1], road_depth / 2 + 0.01, 0, 0,
                                180 / PI * atan(tangent_directions[i][j][1] /
                                                tangent_directions[i][j][0]));
      rs[i].push_back(rs_temp);
    }
  }
}

//find the intersection of bezier-curves with itself and other curves and initialize an intersection at that place
void RoadNetwork::initIntersections() {
  for(int i=0; i<bezier_positions.size(); i++){
    for(int j=i; j<bezier_positions.size(); j++){
      findIntersections(i, j);
    }
  }
}

//check if the mth and nth road intersect and if they do then set up an intersection at that point
void RoadNetwork::findIntersections(int m, int n) {
  for(int i=0; i<bezier_positions[m].size()-1; i++){
    int j;
    if(m!=n)
      j=0;
    else
      j=i+1;
    for(; j<bezier_positions[n].size()-1; j++){
      if(doIntersect(bezier_positions[m][i], bezier_positions[m][i+1], bezier_positions[n][j], bezier_positions[n][j+1])){
        // locate the point of intersection of the paths
        glm::vec2 origin = findOrigin(bezier_positions[m][i], bezier_positions[m][i+1], bezier_positions[n][j], bezier_positions[n][j+1]);
        Intersection temp;
        std::vector<int> type1;
        std::vector<int> type2;
        std::vector<glm::vec2> endpoints;
        /* fill the endpoints(which are suppposed to be arranged in acw sense around origin)
          and classify the roads m and n as type 1(passing completely through intersection) 
          or type 2(which have only one end emerging out of intersection).
          Note that type1, type2 and endpoints are passed by reference*/
        fill_endpoints_type(m, n, origin, i, j, type1, type2, endpoints);
        temp.initIntersection(2*d, road_depth, origin, type1, type2, endpoints);
        intersection.push_back(temp);
      }
    }
  }
}

// find the road-corners at the junction where road ends and intersection starts
// they are the points on the road just greater that 4*d distance away from origin
void RoadNetwork::fill_endpoints_type(int m, int n, glm::vec2 origin, int i, int j, std::vector<int> &type1, std::vector<int> &type2, std::vector<glm::vec2> &endpoints){
  float x = 2*d;   //x is set to be equal to width of the road
  int p, q, r, s;
  // fill in road-corners by traversing away from origin in either direction of the road 
  for(p = i; p>=0; p--){
    float dist = calc_dist(bezier_positions[m][p], origin);
    if(dist>2*x)
      break;
  }
  if(p!=-1){
    endpoints.push_back(road_corners[m][2*p]);
    endpoints.push_back(road_corners[m][2*p+1]);
  }
  for(q = i+1; q<bezier_positions[m].size(); q++){
    float dist = calc_dist(bezier_positions[m][q], origin);
    if(dist>2*x)
      break;
  }
  if(q!=bezier_positions[m].size()){
    endpoints.push_back(road_corners[m][2*q]);
    endpoints.push_back(road_corners[m][2*q+1]);
  }
  for(r = j; r>=0; r--){
    float dist = calc_dist(bezier_positions[n][r], origin);
    if(dist>2*x)
      break;
  }
  if(r!=-1){
    endpoints.push_back(road_corners[n][2*r]);
    endpoints.push_back(road_corners[n][2*r+1]);
  }
  for(s = j+1; s<bezier_positions[n].size(); s++){
    float dist = calc_dist(bezier_positions[n][s], origin);
    if(dist>2*x)
      break;
  }
  if(s!=bezier_positions[m].size()){
    endpoints.push_back(road_corners[n][2*s]);
    endpoints.push_back(road_corners[n][2*s+1]);
  }

  // classify roads as type 1 or type 2
  // if the road has at least one point at a disatnce greater that 4d one either side it is classified as type1
  // else it is classified as type2
  if(p==-1 || q==bezier_positions[m].size()){
    type2.push_back(m);
  }
  else
    type1.push_back(m);
  if(r==-1 || s==bezier_positions[n].size()){
    type2.push_back(n);
  }
  else
    type1.push_back(n);

  // first sort the points of a particular road in ACW sense
  sort_pair_endpoints(endpoints, origin);
  // then sort all of the pairs in ACW sense
  sort_endpoints(endpoints);
}

// sorts the pair of endpoints belonging to a particluar road in ACW sense about origin
void RoadNetwork::sort_pair_endpoints(std::vector<glm::vec2> &endpoints, glm::vec2 origin){
  for(int i=0; i<endpoints.size(); i+=2){
    if(!isACW(endpoints[i], endpoints[i+1], origin)){
      swap(endpoints[i], endpoints[i+1]);
    }
  }
}

// return true if angle v1-origin-v2 is less that 180 measured in anticlockwise direction
bool RoadNetwork::isACW(glm::vec2 v1, glm::vec2 v2, glm::vec2 origin){

  glm::vec2 l1 = v1 - origin;
  glm::vec2 l2 = v2 - origin;

  //for explantion refer to the euler's theorem of rotation of complex numbers
  //float mod = sqrt(l1[0]*l1[0] + l1[1]*l1[1])*sqrt(l2[0]*l2[0] + l2[1]*l2[1]);
  //float Cos = (l1[0]*l2[0] + l1[1]*l2[1])/mod;
  float Sin = (l1[0]*l2[1] - l1[1]*l2[0]);
  if(Sin>0) return true;
  return false; 
}

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int RoadNetwork::orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r) 
{ 
    float val = (q[1] - p[1]) * (r[0] - q[0]) - 
              (q[0] - p[0]) * (r[1] - q[1]); 
  
    if (val == 0) return 0;  // colinear 
    return (val > 0)? 1: 2; // clockwise or counterclock wise 
}

// comparision function to feed to qsort() for arranging points in acw sense
int compare(const void *vp1, const void *vp2) 
{ 
   glm::vec2 *p1 = (glm::vec2 *)vp1; 
   glm::vec2 *p2 = (glm::vec2 *)vp2; 
  
   // Find orientation 
   int o = RoadNetwork::orientation(p0, *p1, *p2); 
   if (o == 0) 
     return (RoadNetwork::calc_dist(p0, *p2) >= RoadNetwork::calc_dist(p0, *p1))? -1 : 1; 
  
   return (o == 2)? -1: 1; 
} 
  

void RoadNetwork::sort_endpoints(std::vector<glm::vec2> &points) 
{ 
   // Find the bottommost point
   bool isaligined = true; 
   float ymin = points[0][1];
   int min = 0; 
   for (int i = 1; i < points.size(); i++) 
   { 
     float y = points[i][1]; 
  
     // Pick the bottom-most. In case of tie, chose the 
     // left most point 
     if ((y < ymin) || (ymin == y && 
         points[i][0] < points[min][0])) 
        ymin = points[i][1], min = i; 
   } 
    if(min%2==1)
      isaligined = false;
   // Place the bottom-most point at first position 
   swap(points[0], points[min]); 
  
   // Sort n-1 points with respect to the first point. 
   // A point p1 comes before p2 in sorted ouput if p2 
   // has larger polar angle (in counterclockwise 
   // direction) than p1 
   p0 = points[0]; //set the data member temporarily to bottom-most point for qsort to work
   qsort(&points[1], points.size()-1, sizeof(glm::vec2), compare); 
   // if the bottom most point was the second in the pair of points of a particular road
   // which were sorted earlier in acw sense already
   // in that case insert the last point in the first position
   if(!isaligined){
     points.insert(points.begin(), points[points.size()-1]);
     points.erase(points.begin() + points.size()-1);
   }
}

//function to swap two vec2s
void RoadNetwork::swap(glm::vec2 &v1, glm::vec2 &v2){
  glm::vec2 temp = v1;
  v1 = v2;
  v2 = temp;
  return;
}

// check if if two lines first formed by points a1 and a2 and second formed by b1 and b2 intersect
bool RoadNetwork::doIntersect(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2){
  float v1  = (b1[1]-a1[1])/(a2[1] - a1[1]) - (b1[0]-a1[0])/(a2[0] - a1[0]);
  float v2  = (b2[1]-a1[1])/(a2[1] - a1[1]) - (b2[0]-a1[0])/(a2[0] - a1[0]);
  float v3  = (a1[1]-b1[1])/(b2[1] - b1[1]) - (a1[0]-b1[0])/(b2[0] - b1[0]);
  float v4  = (a2[1]-b1[1])/(b2[1] - b1[1]) - (a2[0]-b1[0])/(b2[0] - b1[0]);
  // v1*v2 will be negative if b1 and b2 lie on either side of line a1-a2
  // v3*v4 will be negative if a1 and a2 lie on either side of line b2-b2
  // both will be negative if both lines actually intersect in a point 
  return (v1*v2<0 && v3*v4<0);
}


// find the point of intersection of two lines formed by a1-a2 and b1-b2
glm::vec2 RoadNetwork::findOrigin(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2){
  // slope and intersept of the first line
  float m1 = (a2[1]-a1[1])/(a2[0]-a1[0]);
  float c1 = a1[1] -a1[0]*m1;
  // slope and intersept of the second line
  float m2 = (b2[1]-b1[1])/(b2[0]-b1[0]);
  float c2 = b1[1] - b1[0]*m2;
  // the point of intersection (x, y)
  float x = (c2-c1)/(m1-m2);
  float y = m1 * x + c1;
  return glm::vec2(x, y); 
}

void RoadNetwork::mergeCloseIntersections() {
  // traverse through all intersections and find which ones are close to it
  for(int i=0; i<intersection.size(); i++){
    std::vector<int> common_intersections;
    common_intersections.push_back(i);
    for(int j=i+1; j<intersection.size(); j++){
      if(calc_dist(intersection[i].origin, intersection[j].origin)<4*d){
        common_intersections.push_back(j);  // push the index of the intersection if it is close to the ith intersection
      }
    }
    // check if there are more than 1 intersections to merge
    if(common_intersections.size()!=1){
      Intersection common;
      glm::vec2 origin;
      std::vector<int> type1;
      std::vector<int> type2;
      std::vector<glm::vec2> endpoints;
      // set the origin to be the centroid of the polygon formed by origins of constituent intersections 
      // import the type1 and type2 roads from all the constituent-intersections
      for(int j=0; j<common_intersections.size(); j++){
        origin += intersection[common_intersections[j]].origin;
        type1.insert(type1.end(), intersection[common_intersections[j]].type1.begin(), intersection[common_intersections[j]].type1.end());
        type2.insert(type2.end(), intersection[common_intersections[j]].type2.begin(), intersection[common_intersections[j]].type2.end());
      }
      origin /= common_intersections.size();
      //remove duplicates(not only consecutive) from type1 and type2
      std::sort(type1.begin(), type1.end());
      type1.erase(std::unique(type1.begin(), type1.end()), type1.end());
      std::sort(type2.begin(), type2.end());
      type2.erase(std::unique(type2.begin(), type2.end()), type2.end());
      
      // locate the new endpoints of the intersection
      // for type1 roads
      for(int j=0; j<type1.size(); j++){
        for(int k=0; k<bezier_positions[type1[j]].size(); k++){
          if(calc_dist(bezier_positions[type1[j]][k], origin)>4*d && calc_dist(bezier_positions[type1[j]][k+1], origin)<4*d){
            endpoints.push_back(road_corners[type1[j]][2*k]);
            endpoints.push_back(road_corners[type1[j]][2*k+1]);
            break;
          }
        }
        for(int k=bezier_positions[type1[j]].size()-1; k>=0; k--){
          if(calc_dist(bezier_positions[type1[j]][k], origin)>4*d && calc_dist(bezier_positions[type1[j]][k-1], origin)<4*d){
            endpoints.push_back(road_corners[type1[j]][2*k]);
            endpoints.push_back(road_corners[type1[j]][2*k+1]);
            break;
          }
        }
      }
      // for type2 roads
      for(int j=0; j<type2.size(); j++){
        if(calc_dist(bezier_positions[type2[j]][0], origin)<4*d){
          for(int k=bezier_positions[type2[j]].size()-1; k>=0; k--){
            if(calc_dist(bezier_positions[type2[j]][k], origin)>4*d && calc_dist(bezier_positions[type2[j]][k-1], origin)<4*d){
              endpoints.push_back(road_corners[type2[j]][2*k]);
              endpoints.push_back(road_corners[type2[j]][2*k+1]);
              break;
            }
          }
        }
        else{
          for(int k=0; k<bezier_positions[type2[j]].size(); k++){
            if(calc_dist(bezier_positions[type2[j]][k], origin)>4*d && calc_dist(bezier_positions[type2[j]][k+1], origin)<4*d){
              endpoints.push_back(road_corners[type2[j]][2*k]);
              endpoints.push_back(road_corners[type2[j]][2*k+1]);
              break;
            }
          }
        }
      }  
      //sort the endpoints
      sort_pair_endpoints(endpoints, origin);
      sort_endpoints(endpoints);
      //initialize the intersection
      common.initIntersection(2*d, road_depth, origin, type1, type2, endpoints);
      //replace the first of the common_intersection with newly created merged intersection
      intersection[common_intersections[0]] = common;
      //delete the remaining of the common_intersection that have already been merged into one
      for(int k=common_intersections.size()-1; k>=1; k--)
        intersection.erase(intersection.begin() + common_intersections[k]);
    }
  }
}

// check which all roads segments lie inside and intersection and erase them
void RoadNetwork::deleteRoadsInsideIntersection() {
  for(int i=0; i<intersection.size(); i++){
    glm::vec2 origin = intersection[i].origin;
    std::vector<int> type1 = intersection[i].type1;
    std::vector<int> type2 = intersection[i].type2;
    std::vector<int> both_type(type1);
    both_type.insert(both_type.end(), type2.begin() ,type2.end());

    //remove the consecutive-duplicates from this vector which occur in self intersections
    std::vector<int>::iterator ip; 
    ip = std::unique(both_type.begin(), both_type.end()); 
    both_type.resize(std::distance(both_type.begin(), ip));

    //delete those element which lie inside the intersection
    for(int j=0; j<both_type.size(); j++){
      for(int k = (bezier_positions[both_type[j]].size())-1; k>=1; k--){
        if(calc_dist(bezier_positions[both_type[j]][k], origin)>4*d && calc_dist(bezier_positions[both_type[j]][k-1], origin)<4*d){
          r[both_type[j]].erase(r[both_type[j]].begin() + k-1);
        }
        else if(calc_dist(bezier_positions[both_type[j]][k], origin)<4*d)
          r[both_type[j]].erase(r[both_type[j]].begin() + k-1);
      }
    }
  }
}

// erase the separators from the region inside the intersection
void RoadNetwork::deleteRoadSepsInsideIntersection() {
  for (int i = 0; i < bezier_positions.size(); i++) {
    for (int j = 15*((bezier_positions[i].size()-11)/15)+5 , m = rs[i].size()-1; j>=5; j-=15 , m--) {
      for(int k =0; k < intersection.size(); k++) {
        if(calc_dist(bezier_positions[i][j], intersection[k].origin) < 4*d) {
          rs[i].erase(rs[i].begin() + m);
          break;
        }
      }
    }
  }
}

void RoadNetwork::saveIntersections(){
  std::fstream fp;
  // Open .raw file
  fp.open("./models/Intersection-Model/intersection.raw", std::ios::binary | std::ios::out);

  // Check file's condition
  if (!fp.good()) {
    std::cout << "Could not open the intersection.raw file" << std::endl;
    return;
  }

  // Find the size of array of "values to store"
  int cpsize = 0;
  for (int i = 0; i < intersection.size(); i++) {
    cpsize += (intersection[i].type1.size() + intersection[i].type2.size() + intersection[i].endpoints.size() + 3);
  }

  // Generate an array of that size
  glm::vec2 storecp[cpsize + 1];

  // Start the counter
  int countcp = 0;

  // First, store the number of intersections
  storecp[countcp] = glm::vec2(int(intersection.size()), 0);
  countcp++;

  // Recursively store the number of points then the points
  for (int i = 0; i < intersection.size(); i++) {
    storecp[countcp] = intersection[i].origin;
    countcp++;
    storecp[countcp] = glm::vec2(int(intersection[i].type1.size()), 0);
    countcp++;
    storecp[countcp] = glm::vec2(int(intersection[i].type2.size()), 0);
    countcp++;
    storecp[countcp] = glm::vec2(int(intersection[i].endpoints.size()), 0);
    countcp++;
    for (int j = 0; j < intersection[i].type1.size(); j++) {
      storecp[countcp] = glm::vec2(int(intersection[i].type1[j]), 0);
      countcp++;
    }
    for (int j = 0; j < intersection[i].type2.size(); j++) {
      storecp[countcp] = glm::vec2(int(intersection[i].type2[j]), 0);
      countcp++;
    }
    for (int j = 0; j < intersection[i].endpoints.size(); j++) {
      storecp[countcp] = intersection[i].endpoints[j];
      countcp++;
    }
  }

  // Write to the file, then close it
  fp.write((char *)&storecp, sizeof(storecp));
  fp.close();
}

void RoadNetwork::readIntersections(){
  std::fstream fp;

  // Open .raw file
  fp.open("./models/Intersection-Model/intersection.raw", std::ios::binary | std::ios::in);

  // Check file's condition
  if (!fp.good()) {
    std::cout << "Could not read from the intersection.raw file" << std::endl;
    return;
  }

  // Clear the vector storing intersections
  intersection.clear();

  // Find the number of intersections
  glm::vec2 total_intersections;
  fp.read((char *)&total_intersections, sizeof(total_intersections));

  // Recursively find the origin, type1 and type2 roads and the endpoints
  for (int i = 0; i < int(total_intersections[0]); i++) {
    Intersection temp;
    glm::vec2 origin_temp;
    fp.read((char *)&(origin_temp), sizeof(glm::vec2));
    glm::vec2 num_type1, num_type2, num_endpoints;
    fp.read((char *)&num_type1, sizeof(num_type1));
    fp.read((char *)&num_type2, sizeof(num_type2));
    fp.read((char *)&num_endpoints, sizeof(num_endpoints));
    std::vector<int> type1, type2;
    std::vector<glm::vec2> endpoints;
    glm::vec2 cp1[int(num_type1[0])];
    glm::vec2 cp2[int(num_type2[0])];
    glm::vec2 cp3[int(num_endpoints[0])];
    fp.read((char *)&cp1, sizeof(cp1));
    fp.read((char *)&cp2, sizeof(cp2));
    fp.read((char *)&cp3, sizeof(cp3));

    for (int j = 0; j < int(num_type1[0]); j++) {
      type1.push_back(cp1[j][0]);
    }
    for (int j = 0; j < int(num_type2[0]); j++) {
      type2.push_back(cp2[j][0]);
    }
    for (int j = 0; j < int(num_endpoints[0]); j++) {
      endpoints.push_back(cp3[j]);
    }
    // pass the road-width and road-depth as the first two arguments
    // if you also want to render the intersection pass true as the last argument
    temp.initIntersection(0.1, 0.02, origin_temp, type1, type2, endpoints, false);
    intersection.push_back(temp);
  }

  // Close the file
  fp.close();
}


void RoadNetwork::initGraph(){
  for(int i=0; i<intersection.size(); i++){
    g.addVertex(intersection[i].origin);
  }
  for(int i=0; i<bezier_positions.size(); i++) {
    bool a=false, b=false;
    for(int j=0; j<intersection.size(); j++) {
      if(calc_dist(bezier_positions[i][0], intersection[j].origin)<4*d) {
        a = true;
        break;
      }
    }
        
    for(int j=0; j<intersection.size(); j++) {
      if(calc_dist(bezier_positions[i][bezier_positions[i].size()-1], intersection[j].origin)<4*d) {
        b = true;
        break; 
      }
    }
    if(!a) {
      g.addVertex(bezier_positions[i][0]);
    }
    if(!b) {
      g.addVertex(bezier_positions[i][bezier_positions[i].size()-1]);
    }
  }
  for(int i=0; i<bezier_positions.size(); i++) {
    std::vector< std::vector<glm::vec2> > path_points;
    path_points.resize(1);
    for(int j=0; j<bezier_positions[i].size(); j++) {
      bool a = false;
      int k;
      for(k=0; k<intersection.size(); k++)
        if(calc_dist(bezier_positions[i][j], intersection[k].origin)<4*d) {
          a = true;
          break;
        }
      if(!a) {
        path_points[path_points.size()-1].push_back(bezier_positions[i][j]);
      }
      else if(a && j>0 && calc_dist(bezier_positions[i][j-1], intersection[k].origin)>4*d){
        path_points[path_points.size()-1].push_back(bezier_positions[i][j]);
      }
      else {
        while((calc_dist(bezier_positions[i][j], intersection[k].origin)<4*d) && j<bezier_positions[i].size())
          j++;
        if(j==bezier_positions[i].size())
          break;
        else {
          path_points.resize(path_points.size()+1);
          path_points[path_points.size()-1].push_back(bezier_positions[i][j-1]);
        }
      }
    }
    for(int j=0; j<path_points.size(); j++) {
      g.addEdge(path_points[j]);
    }
  }
}

Graph RoadNetwork::getGraph(){
  return g;
}

// render all the intersections on the screen
void RoadNetwork::renderIntersections() {
  for(int i=0; i<intersection.size(); i++){
    intersection[i].render();
  }
}

// returns the unit vector along the input vector
glm::vec2 RoadNetwork::normalize(glm::vec2 v) {
  glm::vec2 n;
  GLfloat f = sqrt(v[0] * v[0] + v[1] * v[1]);
  n[0] = v[0] / f;
  n[1] = v[1] / f;
  return n;
}

// renders the Roads on the screen
void RoadNetwork::renderRoads() {
  for (int i = 0; i < r.size(); i++) {
    for (int j = 0; j < r[i].size(); j++) {
      r[i][j].render();
    }
  }
}

// renders the Roadseps on the screen
void RoadNetwork::renderRoadSeps() {
  for (int i = 0; i < rs.size(); i++) {
    for (int j = 0; j < rs[i].size(); j++) {
      rs[i][j].render();
    }
  }
}

//calculate distance between two vec2s
float RoadNetwork::calc_dist(glm::vec2 v1, glm::vec2 v2) {
  return sqrt((v1[0] - v2[0]) * (v1[0] - v2[0]) + (v1[1] - v2[1]) * (v1[1] - v2[1]));
}

RoadNetwork::~RoadNetwork() {}

} // End namespace soc
