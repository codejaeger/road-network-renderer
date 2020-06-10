#include "road_network/Traffic-Manager/car_node.hpp"
#include "road_network/Bezier-Curve/path.hpp"

namespace soc {

CarNode::CarNode(Graph* graph, std::vector<int> in) {
  // Save the start vertex
  start_vertex_no = in[0];

  path_centered.clear(); // Stores the path
  check_loc_centered.clear(); // Stores the points just before intersections

  // Convert the input of vector of indices to the centered path.
  for (unsigned int i = 1; i < in.size(); i++) {
    if (i % 2) {
      std::vector<glm::vec2> temp = graph->e[in[i]].path;
      for (unsigned int j = 0; j < temp.size(); j++) {
        path_centered.push_back(temp[j]);
      }
      if (int(temp.size()) > 1) {
        temp.pop_back();
        check_loc_centered.push_back(temp.back());
      }
    }
    else {
      path_centered.push_back(graph->v[in[i]].origin);
    }
  }

  if (check_loc_centered.size())
    check_loc_centered.pop_back();

  // bezierCurve(); // To be worked upon.

  // std::cout << "creating car\n";
  // for (unsigned int i = 0; i < path_centered.size(); i++) {
  //   std::cout << path_centered[i][0] << "``" << path_centered[i][1] << std::endl;
  // }
  // std::cout << path_centered.size() << "CarNode created\n";

  assignLane();

  // for (unsigned int i = 0; i < path.size(); i++) {
  //   std::cout << path[i][0] << "``" << path[i][1] << std::endl;
  // }
  // std::cout << path.size() << "CarNode created\n";

  current = -1;

  // Attach a CarModel to the Node
  mod = new CarModel(ROAD_WIDTH * 0.25);
  updateCar();
}

bool CarNode::updateCar() {
  // Increment the current
  current++;

  // If at the end, return false. To be deleted by the Manager
  if (path.size() <= current + 1) {
    return false;
  }

  // Calculating the direction of the car front
  float x1 = path[current][0];
  float y1 = path[current][1];
  float x2 = path[current+1][0];
  float y2 = path[current+1][1];
  glm::vec2 tangent = glm::vec2((x2-x1), (y2-y1));

  float rz;
  if (tangent[0] > 0.0) {
    rz = atan(tangent[1]/tangent[0])*180.0/PI;
  }
  else if (tangent[0] == 0.0) {
    rz = 90.0;
  }
  else {
    rz = 180.0 + (atan(tangent[1]/tangent[0])*180.0/PI);
  }

  // Updating the car model
  mod->change_parameters(path[current][0], path[current][1], ROAD_DEPTH / 2, 0, 0, rz);

  return true;
}

void CarNode::renderCar() {
  // Render the car Model
  // std::cout << path[current][0] << "//" << path[current][1] << "\n";
  mod->render();
}

glm::vec2 CarNode::getLocation() {
  // Returns the current non-centered location
  return path[current];
}

glm::vec2 CarNode::getLocationCentered() {
  // Returns the centered location
  return path_centered[current];
}

std::vector<glm::vec2> CarNode::getCollisionLocations() {
  // Returns the collision locations,
  // i.e. the locations where another car if present.
  // There would be a collision.
  std::vector<glm::vec2> collisionlocs;

  if (path.size() <= current + 3) {
    collisionlocs.push_back(glm::vec2(-50, -50));  // Consider it as a dumping ground.
  }
  else {
    collisionlocs.push_back(path[current+1]);
    collisionlocs.push_back(path[current+2]);
    collisionlocs.push_back(path[current+3]);
  }

  return collisionlocs;
}

bool CarNode::doCheck() {
  // Checks if reaching an intersection
  for (unsigned int i = 0; i < check_loc.size(); i++) {
    if (check_loc[i] == path[current])
      return true;
  }

  return false;
}

void CarNode::assignLane() {
  // Assigns the lane to the car,
  // i.e. converts the centered path to actual path.

  check_loc.resize(check_loc_centered.size());
  path.resize(path_centered.size());
  glm::vec2 normal;
  for (unsigned int i = 0; i < path_centered.size(); i++) {
    if (i + 1 == path_centered.size()) {
      path[i] = path_centered[i] - normal * float(ROAD_WIDTH / 4);
      continue;
    }

    float x1 = path_centered[i][0];
    float y1 = path_centered[i][1];
    float x2 = path_centered[i+1][0];
    float y2 = path_centered[i+1][1];
    float nf = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)); // normalization_factor
    normal = glm::vec2((y2-y1)/nf, (x1-x2)/nf);

    for (unsigned int j = 0; j < check_loc_centered.size(); j++) {
      if (check_loc_centered[j] == path_centered[i])
        check_loc[j] = check_loc_centered[j] - normal * float(ROAD_WIDTH / 4);
    }

    path[i] = path_centered[i] - normal * float(ROAD_WIDTH / 4);
  }
}


void CarNode::bezierCurve() {
  // Has a bug
  // Curves the route at the intersection
  std::vector<glm::vec2> path_new;
  std::vector<glm::vec2> path_temp;
  int flag = 5;

  for (unsigned int i = 0; i < path_centered.size(); i++) {
    for (unsigned int j = 0; j < check_loc_centered.size(); j++) {
      if (check_loc_centered[j] == path_centered[i])
        flag = -1;
    }

    if (flag < 3 && flag >= 0) {
      path_temp.push_back(path_centered[i]);

      if (flag == 2) {
        float n = interpolate_count(path_temp) / 2 + 1; // +1 is to avoid unexpected things
        if (!path_temp.empty()) {
          for (float j = 0; j <= n; j++) {
            std::vector<glm::vec2> pos =
                bezier_curve_point(path_temp, (j / n));
            path_new.push_back(pos[0]);
          }
        }
      }
    }
    else {
      path_new.push_back(path_centered[i]);
    }
    flag++;
  }

  path_centered = path_new;
}

CarNode::~CarNode() {
  // destructor
  std::cout << "CarNode Deleted\n";
  delete mod;
}

} // End namespace soc
