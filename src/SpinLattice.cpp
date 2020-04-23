#include <vector>
#include <random>
#include <iostream>

#include "SpinLattice.h"
#include "MessageQueue.h"

SpinLattice::SpinLattice(int size) :
    _size(size), _lattice(size, std::vector <int> (size, 0))
{
    // initialize the lattice with random spins
    this->setRandomSpins();
}

// Fill the lattice with random spins +1 or -1
void SpinLattice::setRandomSpins(){
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distribution(0, 1);
    for (int i = 0; i < _size; i++){
        for (int j = 0; j < _size; j++){
            // map {0, 1} to {-1, 1}
            _lattice[i][j] = 2 * distribution(rng) - 1;
            std::cout << _lattice[i][j] << " ";
        }
        std::cout << "\n";
    }
}

// Return the average magnetizetion of the lattice 
// m = sum(lattice) / (size*size)
double SpinLattice::getMagnetization(){
    double magnetization = 0;
    for (int i = 0; i < _size; i++){
        for (int j = 0; j < _size; j++){
            magnetization += _lattice[i][j];
        }
    }
    return magnetization / (_size * _size);
}

// Return the energy density of the lattice.
// Energy e_i of the i-th spin is given by:
//
//         e_i = s_i * (-J * B * sum_j(s_j))
//
// where the second sum is evaluated over all neighbors of s_i. 
// J is the coupling constant, B is the external energy field. 
double SpinLattice::getEnergy(double J, double B){
    double energy = 0;
    // count only neighbors above and to the right to avoid double-counting
    int up, right;
    for (int i = 0; i < _size; i++){
        for (int j = 0; j < _size; j++) {
            // wrap the lattice at the borders
            int up = (i + _size - 1) % _size;
            int right = (j + _size + 1) % _size;
            // add up the energy of single spins
            energy = energy + _lattice[j][i] * (-1. * J * (_lattice[up][i] + _lattice[j][right]) + B);
        }
    }
    return energy / (_size * _size);
}

// Flip the spin at position (i, j)
void SpinLattice::flip(int i, int j){
    _lattice[i][j] *= -1;
}

// Use the single cluster method to simulate the time evolution of the lattice.
// Average measurements of energy and magnetizetion for a number of steps given by blockSize and
// send the averages to the queue. 
void SpinLattice::simulate(double J, double B, double T, int blockSize, MessageQueue<double> *queue){
    double energy, magnetization; 

    // prepare a random number generator
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distribution(0, _size-1);

    energy = this->getEnergy(J, B);
    magnetization = this->getMagnetization();

    std::cout << energy << "\n";
    std::cout << magnetization << "\n";

    for (int i = 0; i < _size; i++){
        for (int j = 0; j < _size; j++){
            std::cout << _lattice[i][j] << " ";
        }
        std::cout << "\n";
    }

    /*while(true){
        m = (long int)rand() * size / RAND_MAX;
        n = (long int)rand() * size / RAND_MAX;
        top_el = 0;
        old_spin = spin[m][n];
        cluster[0][0] = m; cluster[0][1] = n;
        while (top_el >= 0){
            m = cluster[top_el][0]; n = cluster[top_el][1];
            cluster[top_el][0] = 0; cluster[top_el][1] = 0;
            top_el--;
            spin[m][n] = -1*old_spin;
            nn_coor[0][0] = (m + size - 1) % size; nn_coor[0][1] = n;
            nn_coor[1][0] = (m + size + 1) % size; nn_coor[1][1] = n;
            nn_coor[2][0] = m;                     nn_coor[2][1] = (n + size + 1) % size;
            nn_coor[3][0] = m;                     nn_coor[3][1] = (n + size - 1) % size;
            for (int k = 0; k < 4; k++){
                j = nn_coor[k][0]; i = nn_coor[k][1];
                if (spin[j][i] == old_spin && (double)rand()/RAND_MAX < prob){
                    top_el++;
                    cluster[top_el][0] = j; cluster[top_el][1] = i;
                    spin[j][i] = -1*old_spin;
                }
            }
        }
    }*/
}