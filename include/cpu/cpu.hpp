#ifndef CPU_H
#define CPU_H

#include "program/program.hpp"

/**
 * The CPU class is an interface for all future emulated CPUs to implement.
 * It includes method for emulating cycles, getPixels to be drawn to the
 * screen, loading programs, and handling IO.
 *
 * @author Anthony Benavente
 * @version 04/13/2016
 */
class Cpu {
public:
    /**
     * This determines whether the CPU is done with updating the grahpics so
     * the graphics handler knows when to update the screen.
     */
	bool drawFlag;

    /**
     * Creates a new CPU object and sets the drawFlag to be false.
     */
	Cpu() : drawFlag(false) {}

    /**
     * This is a method that is meant to be overridden by child classes since
     * each cpu may handle a cycle differently. This method should emulate
     * how the CPU handles a single machine cycle (fetch, execute, repeat).
     */
	virtual void emulateCycle() = 0;
	
    /**
     * This method gets the pixel at the given x and y coordinate. This should
     * be overriden by children classes since different CPUs may store graphics
     * in different locations.
     */
    virtual unsigned int getPixel(int x, int y) = 0;
	
    /**
     * This method loads the specified program into the computer's memory.
     *
     * @param program A pointer to the struct of the program to load into 
     *                memory
     */
    virtual void loadProgram(program_t *program) = 0;
    
    /**
     * This method handles the way that the CPU handles inputs
     *
     * @param port The IO port that the input is entering in
     * @param data The data coming through the port
     */
    virtual void writeIn(uint8_t port, uint8_t data) = 0;
    
    /**
     * This method handles the way that the CPU handles output
     *
     * @param port The port to write out to
     * @return the data that was written
     */
    virtual uint8_t getOut(uint8_t port) = 0;
};

#endif
