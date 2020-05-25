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

  // for (unsigned int i = 0; i < lights.size(); i++) {
  //   for (unsigned int j = 0; j < lights[i]->size; j++) {
  //     e_no_all.push_back(lights[i]->v.outgoing[j]);
  //   }
  // }

}

void Manager::executeManager() {
  time++;

  e_no_go.clear();
  if (time % 5 == 0) {
    for (unsigned int i = 0; i < lights.size(); i++) {
      lights[i]->updateLight();
      e_no_go.push_back(lights[i]->returnEdgeNumber());
    }
  }

  std::vector<Car*> cars_temp;
  for (unsigned int i = 0; i < cars.size(); i++) {
    bool go = true;
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

    if (go) {
      if (cars[i]->updateCar())
        cars_temp.push_back(cars[i]);
      else
        delete cars[i];
    }
    else {
      cars_temp.push_back(cars[i]);
    }
  }
  cars = cars_temp;

  // if (!cars.size() || (time % 5 == 0)) {
  if (!cars.size()) {
    cars.push_back(new Car(g, g->getPath(start, end)));
  }

  std::cout << "\nCars size : " <<  cars.size() << "\t\tTime: " << time << std::endl;
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
