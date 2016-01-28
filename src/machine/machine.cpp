#include "machine/machine.hpp"

#include <SDL2/SDL.h>

void Machine::start() {
	screen.initSDL();

	SDL_Event e;
	while(!quit) {
		while (SDL_PollEvent(&e) != 0) {
			quit = e.type == SDL_QUIT;
		}
		cycle();
	}
}

void Machine::cycle() {
	cpu.emulateCycle();
	screen.update(cpu);
	screen.render();
}
