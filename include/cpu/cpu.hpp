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
    
    virtual void writeIn(uint8_t port, uint8_t data) = 0;
    virtual uint8_t getOut(uint8_t port) = 0;
};

#endif
