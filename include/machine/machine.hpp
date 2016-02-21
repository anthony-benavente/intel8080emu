#ifndef MACHINE_H
#define MACHINE_H

#include "cpu/cpu.hpp"
#include "gfx/screen.hpp"
#include "input/button.hpp"
#include "input/joystick.hpp"

class Machine {
private:
	Cpu& cpu;

	Screen& screen;

	bool quit;

	std::vector<Button> buttons;

	Joystick p1Joystick;
	Joystick p2Joystick;

	void cycle();

public:
	Machine(Cpu& _cpu, Screen& _screen) : cpu(_cpu), screen(_screen),
			quit(false), p1Joystick(1, 0), p2Joystick(2, 1) {
		
	}

	void start();
};

#endif
