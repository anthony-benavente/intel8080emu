CC := g++
TARGET := runner
CFLAGS := -Wall -std=c++11
TESTS_DIR := ../cxxtests/
SRCEXT := cpp
SOURCES := $(shell find ../src -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst ../src/%, ../build/%, $(SOURCES:.$(SRCEXT)=.o))

all:
	make testbuild
	./maketests.sh

testbuild:
	$(TESTS_DIR)/bin/cxxtestgen --error-printer -o testrunner.cpp tests_intel8080.h
	$(CC) $(CFLAGS) -I$(TESTS_DIR) -I../include -DTESTING -c -o testrunner.o testrunner.cpp
