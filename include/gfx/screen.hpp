#ifndef SCREEN_H
#define SCREEN_H
#include <SDL2/SDL.h>
#include "cpu/chip8.hpp"

class Screen {

private:
	SDL_Window *window;

	SDL_Renderer *renderer;

	unsigned int width;

	unsigned int height;

	unsigned int scale;

	unsigned int displayWidth;

	unsigned int displayHeight;

public:
	unsigned int *buffer;

	Screen(unsigned int _width,
		   unsigned int _height,
		   unsigned int _scale);

	~Screen();

	void setPixel(int x, int y, unsigned int val);

	unsigned int getPixel(int x, int y);

	void update(Cpu& cpu);

	void render();

	void initSDL();
};

#endif
