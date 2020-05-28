#include "road_network/Traffic-Manager/car_node.hpp"
#include "road_network/Bezier-Curve/path.hpp"

namespace soc {

CarNode::CarNode(Graph* graph, std::vector<int> in) {
  road_depth = 0.02;
  lane_width = 0.05;

  path_centered.clear(); // Stores the path
  check_loc_centered.clear(); // Stores the points just before intersections

  // std::cout << in.size() << "in\n";
  for (unsigned int i = 0; i < in.size(); i++) {
    // std::cout << in[i] << "--\n";
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

  bezierCurve();
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

  mod = new CarModel(0.020);
  updateCar();
}

bool CarNode::updateCar() {
  current++;

  if (path.size() <= current + 1) {
    return false;
  }

  float x1 = path[current][0];
  float y1 = path[current][1];
  float x2 = path[current+1][0];
  float y2 = path[current+1][1];
  glm::vec2 tangent = glm::vec2((x2-x1), (y2-y1));

  float rz;
  if (tangent[0] > 0.0) {
    rz = atan(tangent[1]/tangent[0])*180.0/PI_MATH;
  }
  else if (tangent[0] == 0.0) {
    rz = 90.0;
  }
  else {
    rz = 180.0 + (atan(tangent[1]/tangent[0])*180.0/PI_MATH);
  }

  mod->change_parameters(path[current][0], path[current][1], road_depth, 0, 0, rz);

  return true;
}

void CarNode::renderCar() {
  // std::cout << path[current][0] << "//" << path[current][1] << "\n";
  mod->render();
}

glm::vec2 CarNode::getLocation() {
  return path[current];
}

glm::vec2 CarNode::getLocationCentered() {
  return path_centered[current];
}

glm::vec2 CarNode::getCollisionLocation() {
  if (path.size() <= current + 2) {
    return glm::vec2(-50, -50);  // Consider it as a dumping ground.
  }
  return path[current+2];
}

bool CarNode::doCheck() {
  for (unsigned int i = 0; i < check_loc.size(); i++) {
    if (check_loc[i] == path[current])
      return true;
  }

  return false;
}

void CarNode::assignLane() {
  check_loc.resize(check_loc_centered.size());
  path.resize(path_centered.size());
  glm::vec2 normal;
  for (unsigned int i = 0; i < path_centered.size(); i++) {
    if (i + 1 == path_centered.size()) {
      path[i] = path_centered[i] - normal*(lane_width/2);
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
        check_loc[j] = check_loc_centered[j] - normal*(lane_width/2);
    }

    path[i] = path_centered[i] - normal*(lane_width/2);
  }
}


void CarNode::bezierCurve() {
  std::vector<glm::vec2> path_new;
  std::vector<glm::vec2> path_temp;
  unsigned int flag = 5;

  for (unsigned int i = 0; i < path_centered.size(); i++) {
    for (unsigned int j = 0; j < check_loc_centered.size(); j++) {
      if (check_loc_centered[j] == path_centered[i])
        flag = 0;
    }

    if (flag < 5) {
      path_temp.push_back(path_centered[i]);

      if (flag == 4) {
        float n = interpolate_count(path_temp) + 1; // +1 is to avoid unexpected things
        for (float j = 0; j <= n; j++) {
          std::vector<glm::vec2> pos =
              bezier_curve_point(path_temp, (j / n));
          path_new.push_back(pos[0]);
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
  std::cout << "CarNode Deleted\n";
  delete mod;
}

}
