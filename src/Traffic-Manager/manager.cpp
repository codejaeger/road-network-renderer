#include "road_network/Traffic-Manager/manager.hpp"

namespace soc {

Manager::Manager(Graph* graph, int s, int e) {
  g = graph;
  start = s;
  end = e;

  time = 0;
}

void Manager::executeManager() {
  time++;

  std::vector<Car*> cars_temp;
  for (unsigned int i = 0; i < cars.size(); i++) {
    if (cars[i]->updateCar())
      cars_temp.push_back(cars[i]);
    else
      delete cars[i];
  }
  cars = cars_temp;

  if (!cars.size() || (time % 5 == 0)) {
    cars.push_back(new Car(g, g->getPath(start, end)));
  }

  std::cout << "\nCars size : " <<  cars.size() << "\t\tTime: " << time << std::endl;
}

void Manager::renderManager() {
  for (unsigned int i = 0; i < cars.size(); i++) {
    std::cout << "\nCar " << i << std::endl;
    cars[i]->renderCar();
  }
}

Manager::~Manager() {
  for (unsigned int i = 0; i < cars.size(); i++) {
    delete cars[i];
  }
  std::cout << "Deleted Manager\n";
}

}
