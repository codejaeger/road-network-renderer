#include "road_network/Traffic-Manager/manager.hpp"

namespace soc {

Manager::Manager(Graph* graph, int s, int e) {
  g = graph;
  start = s;
  end = e;

  time = 0;

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

      // If just before intersection and not a green direction, don't go.
      for (unsigned int j = 0; j < cars.size(); j++) {
        if (j == i)
          continue;
        if (cars[j]->getLocation() == cars[i]->getCollisionLocation()) {
          go = false;
        }
      }

      // If the next location already has a car there.
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

    // If a car is already there at start, don't spawn.
    bool no_new = false;
    for (unsigned int i = 0; i < cars.size(); i++) {
      if (cars[i]->current == 0 || cars[i]->current == 1) {
        no_new = true;
        std::cout << "X\n";
      }
    }

    // CarNode spawning
    if ((time % car_spawnin == 0) && !no_new) {
      cars.push_back(new CarNode(g, g->getPath(start, end)));
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
