//#include <stdlib.h>
//#include <time.h>
//#include <stdarg.h>
//#include <stdbool.h>

#include <fstream>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>
#include <string>

#include "MessageQueue.h"
#include "SpinLattice.h"
#include "evaluate.h"
#include "simulate.h"
#include "display.h"
#include "userInput.h"

int main() {

  int size = 100;                   // size of the lattice (defalt)
  double J = 1.;                    // coupling constant (defalt)
  double B = 0.;                    // external magnetic field (defalt)
  double T = 2.26;                  // temperature (defalt)
  int blockSize = 50;               // number of measurements in each block
  int numBlocks = 100;              // number of subsequent blocks used for error estimation
  std::string inputFileName = "../src/userInput.txt";
  SpinLattice lattice = SpinLattice(size);
  auto queue = MessageQueue<std::vector<double>>();
  std::vector<double> parameters;
  
  // get user input  
  userInput(inputFileName, size, J, B, T);
  parameters = {J, B, T};

  std::cout << "\n";
  std::cout << "=================================================\n";
  std::cout << "Using the single cluster method\n\n";
  std::cout << "Lattice size: " << size << "\n";
  std::cout << "Coupling constant J: " << J << "\n";
  std::cout << "External magnetic field B: " << B << "\n";
  std::cout << "Temperature: " << T << "K\n";
  std::cout << "=================================================\n";
  
  // start the simulation and evaluation
  std::thread simulationThread(&simulate, &lattice, parameters, blockSize, &queue);
  std::thread evaluationThread(&evaluate, parameters, numBlocks, size, &queue);
  std::thread displayThread(&display, &lattice);

  simulationThread.join();
  evaluationThread.join();
  displayThread.join();
}