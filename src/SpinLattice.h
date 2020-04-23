#ifndef STREET_H
#define STREET_H

#include <vector>

class SpinLattice{
    public:
        // constructor / destructor
        SpinLattice(int size);
        ~SpinLattice();

        // getters / setters
        void setRandomSpins();
        double getMagnetization();
        double getEnergy();

        // lattice manipulation
        void flip(int i, int j);


    private:
        std::unique_ptr<std::vector<std::vector<int>>> _lattice;
        int _size;
}

#endif