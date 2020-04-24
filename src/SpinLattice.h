#ifndef SPINLATTICE_H
#define SPINLATTICE_H

#include <memory>
#include <mutex>
#include <vector>

#include "MessageQueue.h"

class SpinLattice {
public:
  // constructor / destructor
  SpinLattice(int size);
  ~SpinLattice() {}

  // getters / setters
  void setRandomSpins();
  double getMagnetization();
  double getEnergy(double J, double B);
  int getSize() { return _size; }
  int getSpin(int i, int j) { return _lattice[i][j]; }
  void setSpin(int i, int j, int s) { _lattice[i][j] = s; }

  // lattice manipulation
  void flip(int i, int j);
  void simulate(std::vector<double> parameters, int blockSize,
                MessageQueue<std::vector<double>> *queue);

private:
  int _size;
  std::vector<std::vector<int>> _lattice;
  std::mutex _mutex;
};

#endif