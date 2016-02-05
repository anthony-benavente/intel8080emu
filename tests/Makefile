CC := g++
TARGET := testrunner
CFLAGS := -g -Wall -std=c++11
TESTS_DIR := ../cxxtests/
SRCEXT := cpp
SOURCES := ../src/cpu/intel8080.cpp ../src/gfx/screen.cpp ../src/machine/machine.cpp ../src/program/program.cpp
OBJECTS := $(patsubst ../src/%, ../build/%, $(SOURCES:.$(SRCEXT)=.o))

all: testbuild testrunner

testbuild:
	$(TESTS_DIR)/bin/cxxtestgen -f --error-printer -o testrunner.cpp tests_intel8080.h
	$(CC) $(CFLAGS) -I$(TESTS_DIR) -I../include -DTESTING -c -o $(TARGET).o $(TARGET).cpp

testrunner:
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) testrunner.o -lSDL2
