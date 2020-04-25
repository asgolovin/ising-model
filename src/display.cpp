#include "display.h"
#include "SpinLattice.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <thread>

void display(SpinLattice *lattice, int waitTime) {
  const char *window_name = "Lattice";
  cv::namedWindow(window_name, cv::WINDOW_NORMAL);
  cv::Mat image(lattice->getSize(), lattice->getSize(), CV_8SC1);

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
    lattice->updateMat(image);
    cv::imshow(window_name, image);
    cv::waitKey(33);
  }
}