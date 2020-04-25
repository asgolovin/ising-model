# 2D Ising model

This is a simulation of the 2D [Ising Model](https://en.wikipedia.org/wiki/Ising_model), a simple model of ferromagnetism in solid state physics, using the single cluster algorithm. [1]

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
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

## A Bit of Phyics

If you take a refrigerator magnet and heat it up, it will suddenly lose its magnetic properties at a specific temperature. But what does temperature have to do with magnetism? 

Some atoms in materials have a magnetic moment - imagine tiny magnets sitting inside the material and interacting with each other. The kind of interaction depends on the properties of the material. In some cases, it is energetically favorable for magnetic moments to align parallel to each other. If the interaction is so strong that most magnetic moments point in the same direction, then the whole material becomes a magnet. 

High temperature introduces chaos into the system. Even if the parallel orientation is energetically favorable, it is just one configuration out of a huge number of possibilities. If the temperature becomes high enough, chaos overcomes order and the material undergoes a phase transition.

The Ising model is a simple model that can describe phase transition from a paramagnetic unodrered state
to a ferromagnetic ordered state. In the model, the magnetic moments are binary and can only point up (+1) or down (-1). Magnetic moments sit on a 2D lattice, their interaction with neighbors is described by the coupling constant `J`. 

* `J > 0`: parallel orientation is preferable
* `J < 0`: antiparallel orientation is preferable

If an external magnetic field `B` is present, magnetic moments react to that and try to align parallel to it. 

If `J > 0` and the temperature is below the critical value, there will be more moments pointing in one direction than the other. 

## Things to Try Out

* If `J = 1.` and `B = 0.`, then the critical temperature for an infinite lattice is approximately equal to `T = 2.26`. The average magnetization density should vanish above that point and be larger than zero below. 
* If `J < 0`, the neighbors will try to point in opposite directions. Such materials are called antiferromagnets. Although the average magnetization cancels out, antiferromagnets still undergo a phase transition from an ordered state to an unordered - it is just not very noticeable from the outside.
* If `B > 0`, the material will be magnetic even at high temperatures. Such materials are called paramagnets.

## To Do

I started with the single cluster method because it is cool and gives fast results, but realized much later that it's very unsuitable for visualization, since many spins flip at once at each step. Well. :D A heat bath algorithm would be prettier. 

## References

[1] Wolff, U. (**1989**). Collective Monte Carlo Updating for Spin Systems _Phys. Rev. Lett., 62,_ 361–364.