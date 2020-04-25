# 2D Ising model

This is a simulation of the 2D [Ising Model](https://en.wikipedia.org/wiki/Ising_model), a simple model of ferromagnetism in materials, using the single cluster method, a.k.a. the Wolff algorithm. [1]

## Dependencies for Running Locally
* ROOT (tested with 6.20/04)
  * All OSes: [click here for installation instructions](https://root.cern/downloading-root)
  * Build instructions can be found [here](https://root.cern/building-root)
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./ising-model`.


## References

[1] Wolff, U. (**1989**). Collective Monte Carlo Updating for Spin Systems _Phys. Rev. Lett., 62,_ 361–364.