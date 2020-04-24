#ifndef SIMULATE_H
#define SIMULATE_H

#include "SpinLattice.h"

// Use the single cluster method to simulate the time evolution of the lattice.
// Average measurements of energy and magnetizetion for a number of steps given
// by blockSize and send the averages to the queue.
void simulate(SpinLattice *lattice, std::vector<double> parameters,
              int blockSize, MessageQueue<std::vector<double>> *queue);

#endif