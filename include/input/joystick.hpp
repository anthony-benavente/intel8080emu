#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "input/input.hpp"

#define JOYSTICK_NONE 	0
#define JOYSTICK_UP 	1
#define JOYSTICK_DOWN 	2
#define JOYSTICK_LEFT 	3
#define JOYSTICK_RIGHT 	4

class Joystick : public Input {
private:
	uint8_t state;

public:
	Joystick(uint16_t id, uint16_t port): Input(id, port),
		state(JOYSTICK_NONE) { }

	void setState(unsigned int);

	void release();

	unsigned int getState() const { return state; }
};

#endif
