#include "road_network/Traffic-Manager/manager.hpp"

namespace soc {

Manager::Manager(Graph* graph, std::string file) {
  g = graph;

  // Data from the file
  std::fstream fp;

  std::string file_name = "./models/Bezier-Model/" + file;
  fp.open(file_name.c_str(), std::ios::binary | std::ios::in);

  if (!fp.good()) {
    std::cout << "could not read from the raw file" << std::endl;
    return;
  }

  glm::vec2 total_pairs;

  fp.read((char *)&total_pairs, sizeof(total_pairs));
  glm::vec2 s[int(total_pairs[0])], e[int(total_pairs[0])];
  fp.read((char *)&s, sizeof(s));
  fp.read((char *)&e, sizeof(e));

  for (unsigned int i = 0; i < (int)(total_pairs[0]); i++) {
    if (s[i] == e[i])
      continue;

    std::cout << e[i][0] << "\t\t" << e[i][1] << "\n";

    for (unsigned int j = 0; j < g->v.size(); j++) {
      std::cout << g->v[j].origin[0] << g->v[j].origin[1] << "mm\n";
      if (s[i] == g->v[j].origin) {
        starts.push_back(int(j));
      }
      else if (e[i] == g->v[j].origin) {
        ends.push_back(int(j));
        std::cout << "Y\n";
      }
    }
  }

  fp.close();

  for (unsigned int i = 0; i < starts.size(); i++) {
    std::cout << starts[i] << "--" << ends[i] << "\n";
  }

  if (!starts.size()) {
    return;
  }

  time = 0;
  spawn_flag = 0;

  for (unsigned int i = 0; i < g->v.size(); i++) {
    if (g->v[i].outgoing.size() > 1) {
      lights.push_back(new IntersectionLights(g, i));
      std::cout << "Pushed\n";
    }
  }

  frame_rate = 40;
  light_timeout_green = 10;
  light_timeout_yellow = 5;
  light_timeout = light_timeout_green + light_timeout_yellow;
  car_spawnin = 2;

}

void Manager::executeManager() {
  if (time % frame_rate == 0) {
    unsigned int clock = time / frame_rate;

    // Update IntersectionLights and store the go directions
    if (clock % light_timeout == 0) {
      edge_firsts_go.clear();
      for (unsigned int i = 0; i < lights.size(); i++) {
        lights[i]->updateLightGreen();
        edge_firsts_go.push_back(lights[i]->returnGoPoint());
      }
    }

    // When green is timed out, the color changes to yellow.
    if (clock % light_timeout == light_timeout_green) {
      edge_firsts_go.clear();
      for (unsigned int i = 0; i < lights.size(); i++) {
        lights[i]->updateLightYellow();
      }
    }

    // for each car
    std::vector<CarNode*> cars_temp;
    std::vector<unsigned int> del_car_ind;
    for (unsigned int i = 0; i < cars.size(); i++) {
      // good to go
      bool go = true;

      std::vector<glm::vec2> collisionlocs = cars[i]->getCollisionLocations();
      for (unsigned int j = 0; j < collisionlocs.size(); j++) {
        // If the next location already has a car there.
        for (unsigned int k = 0; k < cars.size(); k++) {
          if (k == i)
            continue;
          if (cars[k]->getLocation() == collisionlocs[j]) {
            go = false;
          }
        }
      }

      // If just before intersection and not a green direction, don't go.
      if (go) {
        if (cars[i]->doCheck()) {
          // std::cout << "\n\nReached intersection";
          go = false;
          for (unsigned int j = 0; j < edge_firsts_go.size(); j++) {
            if (cars[i]->getLocationCentered() == edge_firsts_go[j]) {
              go = true;
              break;
            }
          }
        }
      }

      // If good to go, update CarNode (move forward)
      if (go) {
        if (cars[i]->updateCar())
          cars_temp.push_back(cars[i]);
        else // If at the end, delete it.
          del_car_ind.push_back(i);
      }
      else {
        cars_temp.push_back(cars[i]);
      }
    }
    for (unsigned int i = 0; i < del_car_ind.size(); i++) {
      delete cars[del_car_ind[i]];
    }
    cars = cars_temp;

    // CarNode spawning
    if ((clock % car_spawnin == 0)) {
      // If a car is already there at start, don't spawn.
      bool no_new = false;
      for (unsigned int i = 0; i < cars.size(); i++) {
        if (cars[i]->start_vertex_no == starts[spawn_flag]) {
          if (cars[i]->current >= 0 && cars[i]->current <= 2) {
            no_new = true;
          }
        }
      }

      if (!no_new) {
        cars.push_back(new CarNode(g, g->getPath(starts[spawn_flag], ends[spawn_flag])));
      }

      spawn_flag++;
      if (spawn_flag == starts.size()) {
        spawn_flag = 0;
      }
    }

    std::cout << "\nCars size : " <<  cars.size() << "\t\tTime: " << time << std::endl;
  }
  time++;
}

void Manager::renderManager() {
  for (unsigned int i = 0; i < cars.size(); i++) {
    // std::cout << "\nCar " << i << std::endl;
    cars[i]->renderCar();
  }

  for (unsigned int i = 0; i < lights.size(); i++) {
    // std::cout << "IntersectionLights " << i << std::endl;
    lights[i]->renderLight();
  }
}

Manager::~Manager() {
  for (unsigned int i = 0; i < cars.size(); i++) {
    delete cars[i];
  }
  for (unsigned int i = 0; i < lights.size(); i++) {
    delete lights[i];
  }
  std::cout << "Deleted Manager\n";
}

}
