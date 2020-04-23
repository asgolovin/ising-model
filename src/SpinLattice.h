#ifndef STREET_H
#define STREET_H

#include <vector>

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


    private:
        int _size;
        std::vector<std::vector<int>> _lattice;
};

#endif