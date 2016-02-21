#include "input/button.hpp"

void Button::trigger() {
	isTriggered = true;
}

void Button::release() {
	isTriggered = false;
}
