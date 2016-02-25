#include "machine/machine.hpp"

#include <SDL2/SDL.h>

void Machine::start() { 
	screen.initSDL();

	SDL_Event e;
    
    keys.insert(std::pair<std::string, bool>("w", false));
    keys.insert(std::pair<std::string, bool>("g", false));
    keys.insert(std::pair<std::string, bool>("h", false));
    keys.insert(std::pair<std::string, bool>("a", false));
    keys.insert(std::pair<std::string, bool>("s", false));
    keys.insert(std::pair<std::string, bool>("d", false));
    keys.insert(std::pair<std::string, bool>("up", false));
    keys.insert(std::pair<std::string, bool>("left", false));
    keys.insert(std::pair<std::string, bool>("down", false));
    keys.insert(std::pair<std::string, bool>("right", false));
    
	while(!quit) {
		while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_w:
                        keys["w"] = e.type == SDL_KEYDOWN;
                        break;
                    case SDLK_a:
                        keys["a"] = e.type == SDL_KEYDOWN;
                        break;
                    case SDLK_s:
                        keys["s"] = e.type == SDL_KEYDOWN;
                        break;
                    case SDLK_d:
                        keys["d"] = e.type == SDL_KEYDOWN;
                        break;
                    case SDLK_UP:
                        keys["up"] = e.type == SDL_KEYDOWN;
                        break;
                    case SDLK_LEFT:
                        keys["left"] = e.type == SDL_KEYDOWN;
                        break;
                    case SDLK_DOWN:
                        keys["down"] = e.type == SDL_KEYDOWN;
                        break;
                    case SDLK_RIGHT:
                        keys["right"] = e.type == SDL_KEYDOWN;
                        break;
                }   
            } else if (e.type == SDL_QUIT) {
			    quit = e.type == SDL_QUIT;
            }
		}
		cycle();
	}
}

void Machine::cycle() {
	cpu.emulateCycle();
	screen.update(cpu);
	screen.render();
}
