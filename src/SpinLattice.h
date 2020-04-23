#ifndef SPINLATTICE_H
#define SPINLATTICE_H

#include <vector>
#include <memory>
#include <mutex>

#include "MessageQueue.h"

class SpinLattice{
    public:
        // constructor / destructor
        SpinLattice(int size);
        ~SpinLattice(){}

        // getters / setters
        void setRandomSpins();
        double getMagnetization();
        double getEnergy(double J, double B);

        // lattice manipulation
        void flip(int i, int j);
        void simulate(double J, double B, double T, int blockSize, MessageQueue<double> *queue);

    private:
        int _size;
        std::vector<std::vector<int>> _lattice;
        std::mutex _mutex;
};

#endif