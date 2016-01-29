#ifndef INTEL8080_H
#define INTEL8080_H

#include "cpu/cpu.hpp"
#include <stack>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

class Intel8080 : public Cpu {

private:
	bool inte;
	uint8 reg[8];
	uint16 pc;
	uint16 sp;
	// uint8 io[0xff + 1];

	std::stack<uint16> stack;
	uint8 memory[0xffff + 1];
	// uint8 gfx[256 * 224];
	uint8 status;

	uint8 getNextOp();
	void decode(uint8 op);

	void NOP();
	void SHLD_A16();
	void STA_A16();
	void RLC();
	void RAL();
	void DAA();
	void STC();
	void RRC();
	void RAR();
	void CMA();
	void CMC();
	void HLT();
	void RNZ();
	void RNC();
	void RPO();
	void RP();
	void JNZ();
	void JNC();
	void JPO();
	void JP();
	void JMP();
	void OUT();
	void XTHL();
	void DI();
	void CNZ();
	void CNC();
	void CPO();
	void CP();
	void ADI();
	void USI();
	void ANI();
	void ORI();
	void RZ();
	void RC();
	void RPE();
	void RM();
	void RET();
	void PCHL();
	void SPHL();
	void JZ();
	void JC();
	void JPE();
	void JM();
	void IN();
	void XCHG();
	void EI();
	void CZ();
	void CC();
	void CPE();
	void CM();
	void CALL();
	void ACI();
	void SBI();

	// 4 types for B, D, H, SP
	void LXI_B();
	void LXI_D();
	void STAX_B();
	void STAX_D();
	void LXI_H();
	void LXI_SP();
	void INX_B();
	void INX_D();
	void INX_H();
	void INX_SP();
	void INR_B();
	void INR_D();
	void INR_H();
	void INR_M();
	void DCR_B();
	void DCR_D();
	void DCR_H();
	void DCR_M();
	void MVI_B();
	void MVI_D();
	void MVI_H();
	void MVI_M();
	void DAD_B();
	void DAD_D();
	void DAD_H();
	void DAD_SP();
	void LDAX_B();
	void LDAX_D();
	void LHLD();
	void LDA();
	void DCX_B();
	void DCX_D();
	void DCX_H();
	void DCX_SP();
	void INR_C();
	void INR_E();
	void INR_L();
	void INR_A();
	void DCR_C();
	void DCR_E();
	void DCR_L();
	void DCR_A();
	void MVI_C();
	void MVI_E();
	void MVI_L();
	void MVI_A();
	void POP_B();
	void POP_D();
	void POP_H();
	void POP_PSW();
	void PUSH_B();
	void PUSH_D();
	void PUSH_H();
	void PUSH_PSW();
	void RST0();
	void RST2();
	void RST4();
	void RST6();
	void RST1();
	void RST3();
	void RST5();
	void RST7();

	void MOV(uint8 from, uint8 to); // Not allowed for M -> M
	void ADD(uint8 reg);
	void ADC(uint8 reg);
	void SUB(uint8 reg);
	void SBB(uint8 reg);
	void ANA(uint8 reg);
	void XRA(uint8 reg);
	void ORA(uint8 reg);
	void CMP(uint8 reg);

public:
	bool drawFlag;

	Intel8080() : inte(false), pc(0), sp(0) {
		for (int i = 0; i <= 0xffff; i++) {
			if (i < 8) {
				reg[i] = 0;
			}
			memory[i] = 0;
		}
	};

	~Intel8080();

	void loadProgram(program_t *program) override;

	void emulateCycle() override;

	unsigned char getPixel(int x, int y) override;

};

#endif
