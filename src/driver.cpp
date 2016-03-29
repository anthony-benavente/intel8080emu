#include <iostream>
#include <SDL2/SDL.h>

#include "cpu/intel8080.hpp"
#include "gfx/screen.hpp"
#include "machine/machine.hpp"
#include "program/program.hpp"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 244
#define SCALE 10

Intel8080 cpu;
Screen screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCALE);

int main(int argc, char **argv) {
	program_t *program = getProgram(argv[1]);
	cpu.loadProgram(program);
	free_program(program);

	Machine machine(cpu, screen);
	machine.start();

	return 0;
}
