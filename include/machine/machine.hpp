#ifndef MACHINE_H
#define MACHINE_H

#include "cpu/cpu.hpp"
#include "gfx/screen.hpp"

class Machine {
private:
	Cpu& cpu;

	Screen& screen;

	bool quit;

	void cycle();

public:
	Machine(Cpu& _cpu, Screen& _screen) : cpu(_cpu), screen(_screen), quit(false) {}

	void start();
};

#endif
