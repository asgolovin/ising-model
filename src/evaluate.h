#ifndef EVALUATE_H
#define EVALUATE_H

#include "MessageQueue.h"
#include <vector>

void evaluate(std::vector<double> parameters, int numBlocks, int size,
              MessageQueue<std::vector<double>> *queue);

double avgDeque(std::deque<double> queue);

#endif