#include "road_network/Traffic-Manager/manager.hpp"

namespace soc {

Manager::Manager(Graph* graph, int s, int e) {
  g = graph;
  start = s;
  end = e;

  time = 0;

  for (unsigned int i = 0; i < g->v.size(); i++) {
    if (g->v[i].outgoing.size() > 1) {
      lights.push_back(new Light(g->v[i]));
      std::cout << "Pushed\n";
    }
  }

  frame_rate = 1;
  light_timeout = 5;
  car_spawnin = 20;
}

void Manager::executeManager() {
  time++;

  if (time % frame_rate == 0) {
    // Update Light and store the green directions
    e_no_go.clear();
    if (time % light_timeout == 0) {
      for (unsigned int i = 0; i < lights.size(); i++) {
        lights[i]->updateLight();
        e_no_go.push_back(lights[i]->returnEdgeNumber());
      }
    }

    // for each car
    std::vector<Car*> cars_temp;
    for (unsigned int i = 0; i < cars.size(); i++) {
      // good to go
      bool go = true;

      // If just before intersection and not a green direction, don't go.
      for (unsigned int j = 0; j < cars.size(); j++) {
        if (j == i)
          continue;
        if (cars[j]->getLocation() == cars[i]->getNextLocation()) {
          go = false;
        }
      }

      // If the next location already has a car there.
      if (cars[i]->doCheck()) {
        std::cout << "\n\nReached intersection";
        go = false;
        for (unsigned int j = 0; j < e_no_go.size(); j++) {
          if (cars[i]->getLocation() == g->e[e_no_go[j]].path[0]) {
            go = true;
            break;
          }
        }
      }

      // If good to go, update Car (move forward)
      if (go) {
        if (cars[i]->updateCar())
          cars_temp.push_back(cars[i]);
        else // If at the end, delete it.
          delete cars[i];
      }
      else {
        cars_temp.push_back(cars[i]);
      }
    }
    cars = cars_temp;

    // If a car is already there at start, don't spawn.
    bool no_new = false;
    for (unsigned int i = 0; i < cars.size(); i++) {
      if (cars[i]->current == 0) {
        no_new = true;
      }
    }

    // Car spawning
    if ((!cars.size() || (time % car_spawnin == 0)) && !no_new) {
      cars.push_back(new Car(g, g->getPath(start, end)));
    }

    std::cout << "\nCars size : " <<  cars.size() << "\t\tTime: " << time << std::endl;
  }

}

void Manager::renderManager() {
  for (unsigned int i = 0; i < cars.size(); i++) {
    std::cout << "\nCar " << i << std::endl;
    cars[i]->renderCar();
  }

  for (unsigned int i = 0; i < lights.size(); i++) {
    std::cout << "Light " << i << std::endl;
    lights[i]->renderLight();
  }
}

Manager::~Manager() {
  for (unsigned int i = 0; i < cars.size(); i++) {
    delete cars[i];
  }
  std::cout << "Deleted Manager\n";
}

}
