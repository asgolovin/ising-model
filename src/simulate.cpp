#include "simulate.h"
#include "SpinLattice.h"
#include <random>
#include <array>
#include <chrono>
#include <thread>

void simulate(SpinLattice *lattice, std::vector<double> parameters,
              int blockSize, MessageQueue<std::vector<double>> *queue) {

    double energyAvg = 0;
    double magnetizationAvg = 0; 
    double J = parameters[0];           // coupling constant
    double B = parameters[1];           // external magnetic field
    double T = parameters[2];           // temperature
    int m, n, i, j;                     // coordinates
    int size = lattice->getSize();      // size of the lattice
    int oldSpin;                        // orientation of the spin at (m, n)
    float prob = 1 - exp(-2 * J/T);     // probability that a spin will be added to cluster
    int counter = 0;                    // counts the number of measurements in block

    // stack for spins which belong to a cluster:
    std::vector<std::array<int, 2>> cluster;

    // coordinates of the neighbors:
    std::array<std::array<int, 2>, 4> nnCoor;

    // prepare a random number generator
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> intDstr(0, size-1);
    std::uniform_real_distribution<float> floatDstr(0, 1);

    while(true){
        counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // select a random spin
        m = intDstr(rng);
        n = intDstr(rng);
        oldSpin = lattice->getSpin(m, n);
        cluster.emplace_back(std::array{m, n});

        // Add all connected spins with the same orientation to the cluster
        while (!cluster.empty()){
            m = cluster.back()[0];
            n = cluster.back()[1];
            cluster.pop_back();
            lattice->setSpin(m, n, -1 * oldSpin);

            // compute coordinates of the neighbors of (i, j)
            nnCoor[0][0] = (m + size - 1) % size;
            nnCoor[0][1] = n;
            nnCoor[1][0] = (m + size + 1) % size; 
            nnCoor[1][1] = n;
            nnCoor[2][0] = m;
            nnCoor[2][1] = (n + size + 1) % size;
            nnCoor[3][0] = m;
            nnCoor[3][1] = (n + size - 1) % size;

            // If a neighbor has the same orientation, add it to the cluster with probability prob
            for (int k = 0; k < 4; k++){
                i = nnCoor[k][0];
                j = nnCoor[k][1];
                if (lattice->getSpin(i, j) == oldSpin && floatDstr(rng) < prob){
                    cluster.emplace_back(std::array{i, j});
                    lattice->setSpin(i, j, -1 * oldSpin);
                }
            }
        }

        // add up magnetization and energy density
        magnetizationAvg += lattice->getMagnetization();
        energyAvg += lattice->getEnergy(J, B);

        // if the required number of measurements for a block is reached:
        if (counter >= blockSize){
            magnetizationAvg /= counter;
            energyAvg /= counter;
            // send the measurements to the evaluation thread
            queue->send(std::vector<double> {magnetizationAvg, energyAvg});

            counter = 0;
            magnetizationAvg = 0.;
            energyAvg = 0.;
        }
    }
}