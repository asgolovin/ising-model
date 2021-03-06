#include "simulate.h"
#include "SpinLattice.h"
#include <cmath>
#include <array>
#include <chrono>
#include <random>
#include <thread>
#include <iostream>

using std::vector;
using std::array;


void simulate(SpinLattice *lattice, vector<double> parameters,
              int blockSize, MessageQueue<vector<double>> *queue, Method method) {

  double energyAvg = 0;             // energy density averaged over a block
  double magnetizationAvg = 0;      // magnetization density averaged over a block
  double J = parameters[0];         // coupling constant
  double B = parameters[1];         // external magnetic field
  double T = parameters[2];         // temperature
  double deltaE;                    // change in energy if a single spin is flipped
  int m, n, i, j;                   // coordinates
  int size = lattice->getSize();    // size of the lattice
  int oldSpin;                      // orientation of the spin at (m, n)
  int counter = 0;                  // counts the number of measurements in block
  vector<array<int, 2>> cluster;    // stack for spins which belong to a cluster:
  array<array<int, 2>, 4> nnCoor;   // coordinates of the neighbors:
  float prob = 1 - exp(-2 * J / T); // probability that a spin will be added to cluster

  // prepare a random number generator
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> intDstr(0, size - 1);
  std::uniform_real_distribution<float> floatDstr(0, 1);

  while (true) {
    counter++;
    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    switch (method){
    case Method::cluster :
      // select a random spin
      m = intDstr(rng);
      n = intDstr(rng);
      oldSpin = lattice->getSpin(m, n);
      cluster.emplace_back(array{m, n});

      // Add all connected spins with the same orientation to the cluster
      while (!cluster.empty()) {
        m = cluster.back()[0];
        n = cluster.back()[1];
        cluster.pop_back();
        lattice->setSpin(m, n, -1 * oldSpin);

        // compute coordinates of the neighbors of (m, n)
        nnCoor = coordinatesOfNeighbors(m, n, size);

        // If a neighbor has the same orientation, add it to the cluster with
        // probability prob
        for (int k = 0; k < 4; k++) {
          i = nnCoor[k][0];
          j = nnCoor[k][1];
          if (lattice->getSpin(i, j) == oldSpin && floatDstr(rng) < prob) {
            cluster.emplace_back(array{i, j});
            lattice->setSpin(i, j, -1 * oldSpin);
          }
        }
      }
      break;

    case Method::heatbath : 
      for (int i = 0; i < size; i++){
          // select a random spin
          m = intDstr(rng);
          n = intDstr(rng);
          oldSpin = lattice->getSpin(m, n);
          deltaE = lattice->deltaEnergy(J, B, m, n);
          (floatDstr(rng) < 1.0/(1.0 + exp(deltaE/T))) ? lattice->setSpin(m, n, 1) : lattice->setSpin(m, n, -1);
      }
      break;
    }

    // add up magnetization and energy density
    magnetizationAvg += std::abs(lattice->getMagnetization());
    energyAvg += lattice->getEnergy(J, B);

    // if the required number of measurements for a block is reached:
    if (counter >= blockSize) {
      // normalize the values
      magnetizationAvg /= counter;
      energyAvg /= counter;
      // send the measurements to the evaluation thread
      queue->send(vector<double>{magnetizationAvg, energyAvg});

      counter = 0;
      magnetizationAvg = 0.;
      energyAvg = 0.;
    }
  }
}

// Returns coodrinates of four neighbors of position (m, n). Wraps at the
// borders
array<array<int, 2>, 4> coordinatesOfNeighbors(int m, int n,
                                                         int size) {
  array<array<int, 2>, 4> nnCoor;

  nnCoor[0] = {(m + size - 1) % size, n};
  nnCoor[1] = {(m + size + 1) % size, n};
  nnCoor[2] = {m, (n + size + 1) % size};
  nnCoor[3] = {m, (n + size - 1) % size};

  return nnCoor;
}

