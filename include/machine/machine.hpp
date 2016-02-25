#ifndef MACHINE_H
#define MACHINE_H

#include <map>
#include <string>

#include "cpu/cpu.hpp"
#include "gfx/screen.hpp"

class Machine {
protected:
	Cpu& cpu;

	Screen& screen;

	bool quit;
    
    std::map<std::string, bool> keys;
    

public:
	Machine(Cpu& _cpu, Screen& _screen) : cpu(_cpu), screen(_screen),
			quit(false){
	}

	void start();
    
    virtual void cycle();
};

#endif
