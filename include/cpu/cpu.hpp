#ifndef CPU_H
#define CPU_H

#include "program/program.hpp"

class Cpu {
public:
	bool drawFlag;
	Cpu() : drawFlag(false) {}

	virtual void emulateCycle() = 0;
	virtual unsigned char getPixel(int x, int y) = 0;
	virtual void loadProgram(program_t *program) = 0;
};

#endif
