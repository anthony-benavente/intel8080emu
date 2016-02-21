#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

class Input {
private:
	uint8_t id;
	uint8_t port;

public:
	Input(uint8_t _id, uint8_t _port) : id(_id), port(_port) { }

	uint8_t getId() const { return id; }
	uint8_t getPort() const { return port; }
};

#endif
