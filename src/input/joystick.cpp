#include "input/joystick.hpp"

void Joystick::setState(unsigned int state) {
	this->state = state;
}

void Joystick::release() {
	state = JOYSTICK_NONE;
}
