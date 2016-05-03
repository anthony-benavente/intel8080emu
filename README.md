Intel 8080 Emulator
===

This project is a Space Invaders/Intel 8080 emulator written in C++. 

## How to Compile

### Tests
To compile the tests, the cxxtests folder must exist in the root of the 
project and must contain the libraries required for cxxtests found in [1]. 
Then, to compile tests, cd into the tests directory and type `make`. This
should make an executable called testrunner which can be run with

    ./testrunner

### Main Program
To compile the project, simply run `make` in the root directory. This should
make the executable in the bin folder called runner.

## How to Run
Simply type

    bin/runner <rom file>

To test running space invaders, make sure you are in the root of the project 
directory, and then type

    bin/runner res/invaders.rom

## Developers
  1. Anthony Benavente

## Resources
  1. https://sourceforge.net/projects/cxxtest/files/cxxtest/
