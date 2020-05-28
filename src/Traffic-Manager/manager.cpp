#include "road_network/Traffic-Manager/manager.hpp"

namespace soc {

Manager::Manager(Graph* graph, std::vector<int> s, std::vector<int> e) {
  g = graph;
  starts = s;
  ends = e;

  time = 0;
  spawn_flag = 0;

  for (unsigned int i = 0; i < g->v.size(); i++) {
    if (g->v[i].outgoing.size() > 1) {
      lights.push_back(new IntersectionLights(g, i));
      std::cout << "Pushed\n";
    }
  }

  frame_rate = 50;
  light_timeout = 200;
  car_spawnin = 200;

}

void Manager::executeManager() {
  if (time % frame_rate == 0) {
    // Update IntersectionLights and store the green directions
    if (time % light_timeout == 0) {
      edge_firsts_go.clear();
      for (unsigned int i = 0; i < lights.size(); i++) {
        lights[i]->updateLight();
        edge_firsts_go.push_back(lights[i]->returnEdgeNumber());
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
    if ((time % car_spawnin == 0)) {
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
