#ifndef INTEL8080_H
#define INTEL8080_H

#include <stdint.h>
#include "cpu/cpu.hpp"

/**
 * The Intel8080 class models an actual Intel 8080 microprocessor.
 *
 * @author Anthony Benavente
 * @version 04/13/2016
 */
class Intel8080 : public Cpu {
// The reason for making this public if TESTING is defined is because our
// tester needs to know about every field and method
#ifdef TESTING
public:
#else
private:
#endif
    int lastInterrupt;

    /**
     * Whether the CPU is terminated.
     */
	bool terminate;

    /**
     * Whether the CPU is halted.
     */
    bool halt;
	
    /**
     * Whether interrupts are enabled or disabled
     */
    bool inte;
    
    /**
     * The value in the interrupt pin, i.e., the address to jump to during an
     * interrupt.
     */
    uint8_t interruptPin;
	
    /**
     * The registers available to the CPU.
     */
    uint8_t reg[8];
	
    /**
     * The program counter -- the current instruction in memory
     */
    uint16_t pc;
	
    /**
     * The stack pointer -- the top of the stack
     */
    uint16_t sp;

    /**
     * The number of cycles that the CPU has gone through total. It loops back
     * around at (2^32 - 1) cycles -- a huge number
     */
    uint32_t cycles;
	
    /**
     * These are all the input ports available to this CPU.
     */
    uint8_t inputs[0xff + 1];
    
    /**
     * All the output ports available to this CPU.
     */
    uint8_t outputs[0xff + 1];

    /**
     * The memory for the CPU. I know that this is tightly coupled, but in the
     * future, this will be pulled out. In fact, let me make a TODO about it.
     *
     * TODO: Decouple memory from the CPU.
     */
	uint8_t memory[0xffff + 1];
	
    /**
     * This is the status byte of the CPU. I'm pretty sure this is actually 
     * implemented in the reg[F] and thus can be deleted. Not sure yet though.
     *
     * TODO: Should this be removed?
     */
    uint8_t status;


    // Yeah for the following instruction methods, I won't be documenting them
    // in detail since we can find a nice, 90+ page document about it somewhere
    // online. There's a link in the README if you must know what each method
    // does.
    
	/************************************
	MOVE STORE LOAD Instructions
	*************************************/
	void MOV_r(uint8_t from, uint8_t to);
	void MOV_r_m(uint8_t from);
	void MOV_m_r(uint8_t to);
	void MVI_m(uint8_t data);
	void MVI_r(uint8_t from, uint8_t data);
	void LXI_r(uint8_t pair, uint16_t data);
	void STAX(uint8_t pair);
	void LDAX(uint8_t pair);
	void STA(uint16_t data);
	void LDA(uint16_t data);
	void SHLD(uint16_t data);
	void LHLD(uint16_t data);
	void XCHG();

	/************************************
	STACK
	*************************************/
	void PUSH(uint8_t pair);
	void POP(uint8_t pair);
	void XTHL();
	void SPHL();
	void LXI_SP(uint16_t data);
	void INX_SP();
	void DCX_SP();

	/************************************
	JUMP
	*************************************/
	void JMP(uint16_t data);
	void JC(uint16_t data);
	void JNC(uint16_t data);
	void JZ(uint16_t data);
	void JNZ(uint16_t data);
	void JP(uint16_t data);
	void JM(uint16_t data);
	void JPE(uint16_t data);
	void JPO(uint16_t data);
	void PCHL();

	/************************************
	CALL
	*************************************/
	void CALL(uint16_t data);
	void CC(uint16_t data);
	void CNC(uint16_t data);
	void CZ(uint16_t data);
	void CNZ(uint16_t data);
	void CP(uint16_t data);
	void CM(uint16_t data);
	void CPE(uint16_t data);
	void CPO(uint16_t data);

	/************************************
	RETURN
	*************************************/
	void RET();
	void RC();
	void RNC();
	void RZ();
	void RNZ();
	void RP();
	void RM();
	void RPE();
	void RPO();

	/************************************
	RESTART
	*************************************/
	void RST(int val);

	/************************************
	INCREMENT AND DECREMENT
	*************************************/
	void INR_r(uint8_t reg);
	void INR_m();
	void DCR_r(uint8_t reg);
	void DCR_m();
	void INX_r(uint8_t pair);
	void DCX_r(uint8_t pair);

	/************************************
	ADD
	*************************************/
	void performAdd(uint8_t data);
	void ADD_r(uint8_t reg);
	void ADD_m();
	void ADC_r(uint8_t reg);
	void ADC_m();
	void ADI(uint8_t data);
	void ACI(uint8_t data);
	void performDAD(uint16_t data);
	void DAD_r(uint8_t pair);
	void DAD_SP();

	/************************************
	SUBTRACT
	*************************************/
	void performSub(uint8_t data);
	void SUB_r(uint8_t reg);
	void SUB_m();
	void SBB_r(uint8_t reg);
	void SBB_m();
	void SUI(uint8_t data);
	void SBI(uint8_t data);

	/************************************
	LOGICAL
	*************************************/
	void performAND(uint8_t data);
	void performXOR(uint8_t data);
	void performOR(uint8_t data);
	void performCMP(uint8_t data);
	void ANA_r(uint8_t reg);
	void ANA_m();
	void XRA_r(uint8_t reg);
	void XRA_m();
	void ORA_r(uint8_t reg);
	void ORA_m();
	void CMP_r(uint8_t reg);
	void CMP_m();
	void ANI(uint8_t data);
	void XRI(uint8_t data);
	void ORI(uint8_t data);
	void CPI(uint8_t data);


	/************************************
	ROTATE
	*************************************/
	void RLC();
	void RRC();
	void RAL();
	void RAR();


	/************************************
	SPECIAL
	*************************************/
	void CMA();
	void STC();
	void CMC();
	void DAA();

	/************************************
	INPUT/OUTPUT
	*************************************/
	void IN(uint8_t data);
	void OUT(uint8_t data);

	/************************************
	CONTROL
	*************************************/
	void EI();
	void DI();
	void NOP();
	void HLT();

    /**
     * Gets the next instruction and increments the program counter
     *
     * @return the next instruction/opcode
     */
	uint8_t getNextOp();
	
    /**
     * Decodes and executes the instruction passed in through the parameter.
     * This moves the pc if the instruction needs following bytes.
     *
     * @param op The opcode to decode
     */
    void decode(uint8_t op);

    /**
     * Getter method to get the program counter
     * 
     * @return the program counter
     */
	uint16_t getPc() { return pc; }

    /**
     * Sets the specific flag to the boolean value found in the second 
     * parameter.
     *
     * @param int The flag to set (see flag constants in Intel8080.cpp)
     * @param int If this value evaluates to anything but 0, the flag is enabled
     */
	void setFlag(int, int);
	
    /**
     * Gets whether the specified flag is enabled (1) or not (0).
     *
     * @return whether the specified flag is enabled
     */
    int getFlag(int);

    /**
     * Gets the register pair H,L as a 16 bit unsigned integer.
     *
     * @return the register pair H,L as a 16 bit unsigned integer
     */
    uint16_t getHL();
	
    /**
     * Resets the flags to their default state
     */
    void resetFlags();
	
    /**
     * Gets the specified register's value
     *
     * @param val The register to get A,F,B,C,D,E,H,L
     * @return the specified register's value
     */
    uint8_t getReg(uint8_t val);
	
    /**
     * Gets the register pair's value as a 16 bit unsigned integer.
     *
     * TODO: This should be uint16_t
     */
    uint8_t getRegPair(uint8_t val);
    
    /**
     * Handles an interrupt.
     */
    void handleInterrupt(uint8_t op);
    
    /**
     * Prints the status of the CPU
     */
    void print_status();
    
    /**
     * Prints the flags all nice like
     */
    void printFlags();

#ifndef TESTING
public:
#endif

	// NOTE: sp starts 1 higher than memory (i.e. 0) bc nothing is in the stack

	Intel8080() : inte(false), pc(0), sp(0), halt(false),
	 	terminate(false), status(0x0) {
		for (int i = 0; i <= 0xffff; i++) {
			if (i < 8) {
				reg[i] = 0;
			}
			memory[i] = 0;
		}
	}

	void loadProgram(program_t *program) override;

	void emulateCycle() override;

	unsigned int getPixel(int x, int y) override;
    
    void writeIn(uint8_t port, uint8_t data) override { inputs[port] = data; }
    
    uint8_t getOut(uint8_t port) override { return outputs[port]; }

};

#endif
