#ifndef INTEL8080_H
#define INTEL8080_H

#include <stdint.h>
#include "cpu/cpu.hpp"

class Intel8080 : public Cpu {
#ifdef TESTING
public:
#else

private:
#endif
	bool terminate;
	bool halt;
	bool inte;
	uint8_t reg[8];
	uint16_t pc;
	uint16_t sp;
	uint32_t cycles;
	uint8_t inputs[0xff + 1];
    uint8_t outputs[0xff + 1];

	uint8_t memory[0xffff + 1];
	uint8_t status;

	uint8_t getNextOp();
	void decode(uint8_t op);
	uint16_t getPc() { return pc; }

	/************************************
	MOVE STORE LOAD
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

	void setFlag(int, int);
	int getFlag(int);
	uint16_t getHL();
	void resetFlags();
	uint8_t getReg(uint8_t val);
	uint8_t getRegPair(uint8_t val);

#ifndef TESTING
public:
#endif

	// NOTE: sp starts 1 higher than memory (i.e. 0) bc nothing is in the stack

	Intel8080() : inte(false), pc(0), sp(0), halt(false),
	 	terminate(false), status(0x42) {
		for (int i = 0; i <= 0xffff; i++) {
			if (i < 8) {
				reg[i] = 0;
			}
			memory[i] = 0;
		}
	}

	void loadProgram(program_t *program) override;

	void emulateCycle() override;

	unsigned char getPixel(int x, int y) override;
    
    void writeIn(uint8_t port, uint8_t data) override { inputs[port] = data; }
    
    uint8_t getOut(uint8_t port) override { return outputs[port]; }

};

#endif
