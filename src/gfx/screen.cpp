#include "gfx/screen.hpp"

#include "cpu/chip8.hpp"

#include <cstdlib>
#include <iostream>

Screen::Screen(unsigned int _width,
			   unsigned int _height,
			   unsigned int _scale) {
	buffer = (unsigned int *) malloc(sizeof(unsigned int) * _width *
	 	_height * _scale);
	width = _width;
	height = _height;
	scale = _scale;
	displayWidth = width * scale;
	displayHeight = height * scale;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			setPixel(x, y, 0);
		}
	}
}

Screen::~Screen() {
	free(buffer);

	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Screen::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error initialize SDL!!!\n");
	} else {
		window = SDL_CreateWindow("Chip8 Emulator - Anthony Benavente",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			displayWidth, displayHeight,
			SDL_WINDOW_SHOWN);
		if (!window) {
			printf("Window could not be created!\n", stderr);
		} else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer);
		}
	}
}

void Screen::setPixel(int x, int y, unsigned int val) {
	buffer[y * width + x] = val;
}

unsigned int Screen::getPixel(int x, int y) {
	return buffer[y * width + x];
}

void Screen::update(Cpu& cpu) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			setPixel(x, y, cpu.getPixel(x, y) * 0xffffff);
		}
	}
}

void Screen::render() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int r = (getPixel(x, y) >> 16) & 0xff;
			int g = (getPixel(x, y) >> 8) & 0xff;
			int b = (getPixel(x, y) & 0xff);
			SDL_Rect rect;
			rect.x = x * scale;
			rect.y = y * scale;
			rect.w = scale;
			rect.h = scale;

			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}
