#include "evaluate.h"
#include <iostream>

void evaluate(std::vector<double> parameters,
              MessageQueue<std::vector<double>> *queue) {
  while (true) {
    auto msg = queue->receive();
    std::cout << "beep! Message from SpinLattice!\n";
    std::cout << "Magnetization: " << msg[0] << "  Energy: " << msg[1] << "\n";
  }
}