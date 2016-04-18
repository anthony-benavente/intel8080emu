#include "machine/spaceinvadersam.hpp"

#define COIN_INSERT 0x1
#define P2_START 0x2
#define P1_START 0x4
#define P1_SHOOT 0x10
#define P1_LEFT 0x20
#define P1_RIGHT 0x40

#define DIPSWITCH_LIVES 0x3
#define TILT 0x4
#define DIPSWITCH_BONUS_LIFE 0x8
#define P2_SHOOT 0x10
#define P2_LEFT 0x20
#define P2_RIGHT 0x40
#define DIPSWITCH_COIN_INFO 0x80

void SpaceInvadersAM::cycle() {
    Machine::cycle();
    
    uint8_t port1 = 0x1;
    uint8_t port2 = 0;
    
    if (keys["h"]) port1 |= P2_START;
    if (keys["g"]) port1 |= P1_START;
    if (keys["a"]) port1 |= P1_SHOOT;
    if (keys["s"]) port1 |= P1_LEFT;
    if (keys["d"]) port1 |= P1_RIGHT;
    
    if (keys["down"]) port2 |= P2_SHOOT;
    if (keys["left"]) port2 |= P2_LEFT;
    if (keys["right"]) port2 |= P2_RIGHT;
    
    cpu.writeIn(1, port1);
    cpu.writeIn(2, port2);
    
    shreg.srl(cpu.getOut(4));
    shreg.setOffset(cpu.getOut(2));
    cpu.writeIn(3, shreg.getOutput());
}