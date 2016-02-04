#!/bin/sh
make testbuild
g++ -g -Wall -o testrunner ../build/program/program.o ../build/gfx/screen.o ../build/cpu/intel8080.o ../build/machine/machine.o testrunner.o -lSDL2
