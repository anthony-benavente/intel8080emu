#ifndef BUTTON_H
#define BUTTON_H

#include "input/input.hpp"

class Button : public Input {
private:
	bool isTriggered;

public:
	Button(uint16_t _id, uint16_t _port) : Input(_id, _port),
		isTriggered(false) { }

	void triggerIf(bool condition) {
		if (condition) {
			trigger();
		} else {
			release();
		}
	}

	void trigger();

	void release();
};

#endif
