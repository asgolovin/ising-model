#ifndef EVALUATE_H
#define EVALUATE_H

#include "MessageQueue.h"
#include <vector>

void evaluate(std::vector<double> parameters,
              MessageQueue<std::vector<double>> *queue);

#endif