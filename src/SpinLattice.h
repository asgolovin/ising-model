#ifndef SPINLATTICE_H
#define SPINLATTICE_H

#include <memory>
#include <mutex>
#include <opencv2/core.hpp>
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
  double deltaEnergy(double J, double B, int i, int j);

  // lattice manipulation
  void flip(int i, int j);

  // graphics
  void updateMat(cv::Mat &image);

private:
  int _size;
  std::mutex _mutex;
  std::vector<std::vector<int>> _lattice;
};

#endif