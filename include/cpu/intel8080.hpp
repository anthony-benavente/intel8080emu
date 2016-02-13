#ifndef INTEL8080_H
#define INTEL8080_H

#include "cpu/cpu.hpp"
// #include <stack>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

class Intel8080 : public Cpu {
#ifdef TESTING
public:
#else

private:
#endif

	bool inte;
	uint8 reg[8];
	uint16 pc;
	uint16 sp;
	uint32 cycles;
	// uint8 io[0xff + 1];

	// std::stack<uint16> stack;
	uint8 memory[0xffff + 1];
	// uint8 gfx[256 * 224];
	uint8 status;

	uint8 getNextOp();
	void decode(uint8 op);
	uint16 getPc() { return pc; }

	/************************************
	MOVE STORE LOAD
	*************************************/
	void MOV_r(uint8 from, uint8 to);
	void MOV_r_m(uint8 from);
	void MOV_m_r(uint8 to);
	void MVI_m(uint8 data);
	void MVI_r(uint8 from, uint16 data);
	void LXI_r(uint8 pair, uint16 data);
	void STAX(uint8 pair);
	void LDAX(uint8 pair);
	void STA(uint16 data);
	void LDA(uint16 data);
	void SHLD(uint16 data);
	void LHLD(uint16 data);
	void XCHG();

	/************************************
	STACK
	*************************************/
	void PUSH(uint8 pair);
	void POP(uint8 pair);
	void XTHL();
	void SPHL();
	void LXI_SP(uint16 data);
	void INX_SP();
	void DCX_SP();

	/************************************
	JUMP
	*************************************/
	void JMP(uint16 data);
	void JC(uint16 data);
	void JNC(uint16 data);
	void JZ(uint16 data);
	void JNZ(uint16 data);
	void JP(uint16 data);
	void JM(uint16 data);
	void JPE(uint16 data);
	void JPO(uint16 data);
	void PCHL();

	/************************************
	CALL
	*************************************/
	void CALL(uint16 data);
	void CC(uint16 data);
	void CNC(uint16 data);
	void CZ(uint16 data);
	void CNZ(uint16 data);
	void CP(uint16 data);
	void CM(uint16 data);
	void CPE(uint16 data);
	void CPO(uint16 data);

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
	void INR_r(uint8 reg);
	void INR_m();
	void DCR_r(uint8 reg);
	void DCR_m();
	void INX_r(uint8 pair);
	void DCX_r(uint8 pair);

	/************************************
	ADD
	*************************************/
	void performAdd(uint8 data);
	void ADD_r(uint8 reg);
	void ADD_m();
	void ADC_r(uint8 reg);
	void ADC_m();
	void ADI(uint8 data);
	void ACI(uint8 data);
	void performDAD(uint16 data);
	void DAD_r(uint8 pair);
	void DAD_SP();

	/************************************
	SUBTRACT
	*************************************/
	void performSub(uint8 data);
	void SUB_r(uint8 reg);
	void SUB_m();
	void SBB_r(uint8 reg);
	void SBB_m();
	void SUI(uint8 data);
	void SBI(uint8 data);

	/************************************
	LOGICAL
	*************************************/
	void performAND(uint8 data);
	void performXOR(uint8 data);
	void performOR(uint8 data);
	void performCMP(uint8 data);
	void ANA_r(uint8 reg);
	void ANA_m();
	void XRA_r(uint8 reg);
	void XRA_m();
	void ORA_r(uint8 reg);
	void ORA_m();
	void CMP_r(uint8 reg);
	void CMP_m();
	void ANI(uint8 data);
	void XRI(uint8 data);
	void ORI(uint8 data);
	void CPI(uint8 data);

	void NOP();
	void RLC();
	void RAL();
	void DAA();
	void STC();
	void RRC();
	void RAR();
	void CMA();
	void CMC();
	void HLT();
	void OUT();
	void DI();
	void IN();
	void EI();

	// 4 types for B, D, H, SP
	void MVI_B();
	void MVI_D();
	void MVI_H();
	void MVI_M();
	void INR_C();
	void INR_E();
	void INR_L();
	void INR_A();
	void DCR_C();
	void DCR_E();
	void DCR_L();
	void DCR_A();

	void setFlag(int, int);
	int getFlag(int);
	uint16 getHL();
	void resetFlags();

#ifndef TESTING
public:
#endif

	// NOTE: sp starts 1 higher than memory bc nothing is in the stack

	Intel8080() : inte(false), pc(0), sp(0xffff + 1) {
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

};

#endif
