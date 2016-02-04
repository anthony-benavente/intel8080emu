#include "cpu/intel8080.hpp"

#define B 0
#define C 1
#define D 2
#define E 3
#define H 4
#define L 5
#define M 6
#define A 7

#define STATUS_CARRY 		0x01
#define STATUS_PARITY 		0x04
#define STATUS_AUX_CARRY 	0x10
#define STATUS_SIGN 		0x80
#define STATUS_ZERO 		0x40

void Intel8080::loadProgram(program_t *program) {
	for (int i = 0; i < program->size; i++) {
		memory[i] = program->data[i];
	}
}

void Intel8080::emulateCycle() {
	uint8 op = getNextOp();
	pc += 2;
	decode(op);
}

unsigned char Intel8080::getPixel(int x, int y) {
	return 0;
}

unsigned char Intel8080::getNextOp() {
	return memory[pc];
}

void Intel8080::decode(uint8 op) {
}
void Intel8080::NOP() {
}
void Intel8080::SHLD_A16() {
}
void Intel8080::STA_A16() {
}
void Intel8080::RLC() {
}
void Intel8080::RAL() {
}
void Intel8080::DAA() {
}
void Intel8080::STC() {
}
void Intel8080::RRC() {
}
void Intel8080::RAR() {
}
void Intel8080::CMA() {
}
void Intel8080::CMC() {
}
void Intel8080::HLT() {
}
void Intel8080::RNZ() {
}
void Intel8080::RNC() {
}
void Intel8080::RPO() {
}
void Intel8080::RP() {
}
void Intel8080::JNZ() {
}
void Intel8080::JNC() {
}
void Intel8080::JPO() {
}
void Intel8080::JP() {
}
void Intel8080::JMP() {
}
void Intel8080::OUT() {
}
void Intel8080::XTHL() {
}
void Intel8080::DI() {
}
void Intel8080::CNZ() {
}
void Intel8080::CNC() {
}
void Intel8080::CPO() {
}
void Intel8080::CP() {
}
void Intel8080::ADI() {
}
void Intel8080::USI() {
}
void Intel8080::ANI() {
}
void Intel8080::ORI() {
}
void Intel8080::RZ() {
}
void Intel8080::RC() {
}
void Intel8080::RPE() {
}
void Intel8080::RM() {
}
void Intel8080::RET() {
}
void Intel8080::PCHL() {
}
void Intel8080::SPHL() {
}
void Intel8080::JZ() {
}
void Intel8080::JC() {
}
void Intel8080::JPE() {
}
void Intel8080::JM() {
}
void Intel8080::IN() {
}
void Intel8080::XCHG() {
}
void Intel8080::EI() {
}
void Intel8080::CZ() {
}
void Intel8080::CC() {
}
void Intel8080::CPE() {
}
void Intel8080::CM() {
}
void Intel8080::CALL() {
}
void Intel8080::ACI() {
}
void Intel8080::SBI() {
}
void Intel8080::LXI_B() {
}
void Intel8080::LXI_D() {
}
void Intel8080::STAX_B() {
}
void Intel8080::STAX_D() {
}
void Intel8080::LXI_H() {
}
void Intel8080::LXI_SP() {
}
void Intel8080::INX_B() {
}
void Intel8080::INX_D() {
}
void Intel8080::INX_H() {
}
void Intel8080::INX_SP() {
}
void Intel8080::INR_B() {
}
void Intel8080::INR_D() {
}
void Intel8080::INR_H() {
}
void Intel8080::INR_M() {
}
void Intel8080::DCR_B() {
}
void Intel8080::DCR_D() {
}
void Intel8080::DCR_H() {
}
void Intel8080::DCR_M() {
}
void Intel8080::MVI_B() {
}
void Intel8080::MVI_D() {
}
void Intel8080::MVI_H() {
}
void Intel8080::MVI_M() {
}
void Intel8080::DAD_B() {
}
void Intel8080::DAD_D() {
}
void Intel8080::DAD_H() {
}
void Intel8080::DAD_SP() {
}
void Intel8080::LDAX_B() {
}
void Intel8080::LDAX_D() {
}
void Intel8080::LHLD() {
}
void Intel8080::LDA() {
}
void Intel8080::DCX_B() {
}
void Intel8080::DCX_D() {
}
void Intel8080::DCX_H() {
}
void Intel8080::DCX_SP() {
}
void Intel8080::INR_C() {
}
void Intel8080::INR_E() {
}
void Intel8080::INR_L() {
}
void Intel8080::INR_A() {
}
void Intel8080::DCR_C() {
}
void Intel8080::DCR_E() {
}
void Intel8080::DCR_L() {
}
void Intel8080::DCR_A() {
}
void Intel8080::MVI_C() {
}
void Intel8080::MVI_E() {
}
void Intel8080::MVI_L() {
}
void Intel8080::MVI_A() {
}
void Intel8080::POP_B() {
}
void Intel8080::POP_D() {
}
void Intel8080::POP_H() {
}
void Intel8080::POP_PSW() {
}
void Intel8080::PUSH_B() {
}
void Intel8080::PUSH_D() {
}
void Intel8080::PUSH_H() {
}
void Intel8080::PUSH_PSW() {
}
void Intel8080::RST0() {
}
void Intel8080::RST2() {
}
void Intel8080::RST4() {
}
void Intel8080::RST6() {
}
void Intel8080::RST1() {
}
void Intel8080::RST3() {
}
void Intel8080::RST5() {
}
void Intel8080::RST7() {
}
void Intel8080::MOV(uint8 from, uint8 to) {
}
void Intel8080::ADD(uint8 reg) {
}
void Intel8080::ADC(uint8 reg) {
}
void Intel8080::SUB(uint8 reg) {
}
void Intel8080::SBB(uint8 reg) {
}
void Intel8080::ANA(uint8 reg) {
}
void Intel8080::XRA(uint8 reg) {
}
void Intel8080::ORA(uint8 reg) {
}
void Intel8080::CMP(uint8 reg) {
}
