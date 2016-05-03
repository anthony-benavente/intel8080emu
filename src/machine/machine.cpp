#include "machine/machine.hpp"

#include <pthread.h>
#include <semaphore.h>
#include <cpu/intel8080.hpp>
#include <SDL2/SDL.h>

void *doCpuWork(void *arg) {
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    
    struct timespec sleeptime;
    sleeptime.tv_sec = 0;
    sleeptime.tv_nsec = 8000;
    
    if (arg != NULL) {
        Intel8080 *cpu = (Intel8080 *) arg;
        // Update the CPU
        while (1) {
            cpu->emulateCycle();
            nanosleep(&sleeptime, NULL);
        }   
    }
}

void Machine::start() {    
    pthread_t cpuThread;
    pthread_create(&cpuThread, NULL, doCpuWork, (void *) &cpu);
    
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
    
    // SDL happens on a separate thread from the CPU
	while(!quit) {
		while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                printf("Key was pressed!\n");
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
                    case SDLK_h:
                        keys["g"] = e.type == SDL_KEYDOWN;
                        break;
                    case SDLK_g:
                        keys["h"] = e.type == SDL_KEYDOWN;
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
	screen.update(cpu);
	screen.render();
}
