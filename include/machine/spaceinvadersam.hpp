#ifndef SPACEINVADERSAM_H
#define SPACEINVADERSAM_H

#import <stack>

#import "machine/machine.hpp"
#import "input/bitshifter.hpp"

class SpaceInvadersAM : public Machine {   
private:
    Bitshifter shreg;
    
public:
    SpaceInvadersAM(Cpu& _cpu, Screen& _screen) : Machine(_cpu, _screen) {
    }
    
    void cycle() override;  
};

#endif
