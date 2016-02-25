#ifndef BITSHIFTER_H
#define BITSHIFTER_H

#include "input/input.hpp"

class Bitshifter : public Input {
private:
    uint16_t val;
    uint8_t output;
    uint8_t offset;
    
public:
    Bitshifter() : Input(0, 4), val(0), output(0), offset(0) {
    }

    void srl(uint8_t shval) {
        val >>= 8;
        val |= (shval << 8);
    }

    void sll(uint8_t shval) {
        val <<= 8;
        val |= shval;
    }
    
    void setOffset(uint8_t _offset) { offset = _offset; }
    uint8_t getOffset() const { return offset; }
    uint8_t getOutput() { return (val >> offset) & 0xff; }
};

#endif