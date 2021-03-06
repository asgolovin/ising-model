#ifndef SIMULATE_H
#define SIMULATE_H

#include "SpinLattice.h"

enum Method { cluster, heatbath };

// Use the single cluster method to simulate the time evolution of the lattice.
// Average measurements of energy and magnetizetion for a number of steps given
// by blockSize and send the averages to the queue.
void simulate(SpinLattice *lattice, std::vector<double> parameters,
              int blockSize, MessageQueue<std::vector<double>> *queue, Method method);


std::array<std::array<int, 2>, 4> coordinatesOfNeighbors(int m, int n, int size);

#endif