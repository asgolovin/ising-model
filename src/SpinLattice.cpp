#include <iostream>
#include <mutex>
#include <opencv2/core.hpp>
#include <random>
#include <vector>

#include "MessageQueue.h"
#include "SpinLattice.h"

SpinLattice::SpinLattice(int size)
    : _size(size), _lattice(size, std::vector<int>(size, 1)) {
  // initialize the lattice with random spins
  this->setRandomSpins();
}

// Fill the lattice with random spins +1 or -1
void SpinLattice::setRandomSpins() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> distribution(0, 1);
  std::lock_guard<std::mutex> lock(_mutex);
  for (int i = 0; i < _size; i++) {
    for (int j = 0; j < _size; j++) {
      // map {0, 1} to {-1, 1}
      _lattice[i][j] = 2 * distribution(rng) - 1;
    }
  }
}

// Return the average magnetizetion of the lattice
// m = sum(lattice) / (size*size)
double SpinLattice::getMagnetization() {
  std::lock_guard<std::mutex> lock(_mutex);
  double magnetization = 0;
  for (int i = 0; i < _size; i++) {
    for (int j = 0; j < _size; j++) {
      magnetization += _lattice[i][j];
    }
  }
  return magnetization / (_size * _size);
}

// Return the energy density of the lattice.
// Energy e_i of the i-th spin is given by:
//    e_i = s_i * (-J * B * sum_j(s_j))
// where the second sum is evaluated over all neighbors of s_i.
// J is the coupling constant, B is the external energy field.
double SpinLattice::getEnergy(double J, double B) {
  std::lock_guard<std::mutex> lock(_mutex);
  double energy = 0;
  // count only neighbors above and to the right to avoid double-counting
  int up, right;
  for (int i = 0; i < _size; i++) {
    for (int j = 0; j < _size; j++) {
      // wrap the lattice at the borders
      int up = (i + _size - 1) % _size;
      int right = (j + _size + 1) % _size;
      // add up the energy of single spins
      energy += _lattice[i][j] *
                (-1. * J * (_lattice[up][j] + _lattice[i][right]) + B);
    }
  }
  return energy / (_size * _size);
}

// Flip the spin at position (i, j)
void SpinLattice::flip(int i, int j) { _lattice[i][j] *= -1; }

// Updates an OpenCV 8-bit 1 channel matrix (CV_8SC1)
void SpinLattice::updateMat(cv::Mat &image) {
  std::lock_guard<std::mutex> lock(_mutex);
  for (int i = 0; i < _size; ++i) {
    for (int j = 0; j < _size; ++j) {
      image.at<schar>(i, j) = _lattice[i][j] * 127;
    }
  }
}