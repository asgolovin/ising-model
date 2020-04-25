#include "evaluate.h"
#include <cmath>
#include <deque>
#include <iostream>
#include <thread>

void evaluate(std::vector<double> parameters, int numBlocks, int size,
              MessageQueue<std::vector<double>> *queue) {

  double J = parameters[0];             // coupling constant
  double B = parameters[1];             // external magnetic field
  double T = parameters[2];             // temperature
  double varEnergy;                     // variance of the energy
  double varHeat;                       // variance of the heat capacity
  double varMagnetization;              // variance of the magnetization density
  double varSusc;                       // variance of the susceptibility
  double heat;                          // average heat capacity
  double susc;                          // average susceptibility
  std::deque<double> magnetization;     // most recent measurements of magnetization density
  std::deque<double> energy;            // most recent measurements of energy density
  std::vector<double> energyJK;         // array for Jackknife analysis
  std::vector<double> magnetizationJK;  // array for Jackknife analysis
  double magnetizationAvg;              // average magnetization
  double energyAvg;                     // average energy

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    auto msg = queue->receive();

    // save all measurements from the queue
    for (int i = 0; i < msg.size(); i++) {
      magnetization.emplace_back(msg[i][0]);
      energy.emplace_back(msg[i][1]);
    }

    // wait till there are enough measurements
    if (energy.size() < numBlocks) {
      std::cout << "Waiting for measurements... \n";
      continue;
    }

    // keep only the most recent measurements
    while (energy.size() > numBlocks) {
      magnetization.pop_front();
      energy.pop_front();
    }

    // compute average values
    magnetizationAvg = avgDeque(magnetization);
    energyAvg = avgDeque(energy);

    // Jackknife error estimation

    varEnergy = 0, varHeat = 0, varMagnetization = 0, varSusc = 0;
    heat = 0, susc = 0;
    energyJK.clear();
    magnetizationJK.clear();

    for (int s = 0; s < numBlocks; s++) {
      energyJK.emplace_back(0);
      magnetizationJK.emplace_back(0);

      for (int i = 0; i < numBlocks; i++) {
        if (i == s) {
          continue;
        }
        energyJK.back() += energy[i];
        magnetizationJK.back() += magnetization[i];
      }

      energyJK.back() /= (numBlocks - 1);
      magnetizationJK.back() /= (numBlocks - 1);
      heat += pow((energyJK.back() - energyAvg), 2) / numBlocks;
      susc += pow((magnetizationJK.back() - magnetizationAvg), 2) / numBlocks;
    }

    heat *= (pow(size, 4) / (T * T));
    susc *= (pow(size, 4) / T);

    for (int s = 0; s < numBlocks; s++) {
      varEnergy += pow((energyJK[s] - energyAvg), 2);
      varMagnetization += pow((magnetizationJK[s] - magnetizationAvg), 2);
      varHeat += pow(pow((energyJK[s] - energyAvg), 2) * pow(size, 4) / (T * T) - heat, 2);
      varSusc += pow(pow((magnetizationJK[s] - magnetizationAvg), 2) * pow(size, 4) / T - susc, 2);
    }

    varEnergy *= (numBlocks - 1.) / numBlocks;
    varMagnetization *= (numBlocks - 1.) / numBlocks;
    varHeat *= (numBlocks - 1.) / numBlocks;
    varSusc *= (numBlocks - 1.) / numBlocks;

    std::cout << "\n";
    printf("Average energy density:        % 6f +/- %6f\n", energyAvg, sqrt(varEnergy));
    printf("Average magnetisation density: % 6f +/- %6f\n", magnetizationAvg, sqrt(varMagnetization));
    printf("Heat capacity:                 % 6f +/- %6f\n", heat, sqrt(varHeat));
    printf("Susceptibility:                % 6f +/- %6f\n", susc, sqrt(varSusc));
  }
}

double avgDeque(std::deque<double> queue) {
  double sum = 0;
  for (double x : queue) {
    sum += x;
  }
  return sum / queue.size();
}