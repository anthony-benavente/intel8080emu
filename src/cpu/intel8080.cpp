#include "cpu/intel8080.hpp"

#include <iostream>
using std::cout;
using std::endl;

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

void swap(uint8 *, int, int);

void swap(uint8 *arr, int a, int b) {
	uint8 tmp = *(arr + a);
	*(arr + a) = *(arr + b);
	*(arr + b) = tmp;
}

void Intel8080::loadProgram(program_t *program) {
	for (int i = 0; i < program->size; i++) {
		memory[i] = program->data[i];
	}
}

void Intel8080::emulateCycle() {
	uint8 op = getNextOp();
	decode(op);
}

unsigned char Intel8080::getPixel(int x, int y) {
	return 0;
}

uint8 Intel8080::getNextOp() {
	return memory[pc++];
}

void Intel8080::decode(uint8 op) {
}
void Intel8080::NOP() {
}

void Intel8080::RST(int val) {
	stack.push(pc);
	pc = val;
}

void Intel8080::MOV_r(uint8 from, uint8 to) {
	uint8 tmp = reg[to];
	reg[to] = reg[from];
	cycles += 5;
}
void Intel8080::MOV_r_m(uint8 from) {
	uint16 memIndex = (reg[H] << 8) | reg[L];
	memory[memIndex] = reg[from];
	cycles += 7;
}
void Intel8080::MOV_m_r(uint8 to) {
	uint16 memIndex = (reg[H] << 8) | reg[L];
	reg[to] = memory[memIndex];
	cycles += 7;
}
void Intel8080::MVI_m(uint8 data) {
	uint16 memIndex = (reg[H] << 8) | reg[L];
	memory[memIndex] = data;
	cycles += 7;
}
void Intel8080::MVI_r(uint8 from, uint16 data) {
	reg[from] = data;
	cycles += 7;
}
void Intel8080::LXI_r(uint8 pair, uint16 data) {
	if (pair == B || pair == D || pair == H) {
		reg[pair] = data & 0xff;
		reg[pair + 1] = (data >> 8) & 0xff;
	}
	cycles += 10;
}
void Intel8080::STAX(uint8 pair) {
	if (pair == B || pair == D) {
		uint16 memIndex = (reg[pair] << 8) | reg[pair + 1];
		memory[memIndex] = reg[A];
	}
	cycles += 7;
}
void Intel8080::LDAX(uint8 pair) {
	if (pair == B || pair == D) {
		uint16 memIndex = (reg[pair] << 8) | reg[pair + 1];
		reg[A] = memory[memIndex];
	}
	cycles += 7;
}
void Intel8080::STA(uint16 data) {
	uint8 low = (data >> 8) & 0xff;
	uint8 high = data & 0xff;
	uint16 memIndex = (low << 8) | high;
	memory[memIndex] = reg[A];
	cycles += 13;
}
void Intel8080::LDA(uint16 data) {
	reg[A] = memory[data];
	cycles += 13;
}
void Intel8080::SHLD(uint16 data) {
	memory[data] = reg[L];
	memory[data + 1] = reg[H];
	cycles += 16;
}
void Intel8080::LHLD(uint16 data) {
	reg[L] = memory[data];
	reg[H] = memory[data + 1];
	cycles += 16;
}
void Intel8080::XCHG() {
	swap(reg, D, H);
	swap(reg, E, L);
	cycles += 4;
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
void Intel8080::DAD_B() {
}
void Intel8080::DAD_D() {
}
void Intel8080::DAD_H() {
}
void Intel8080::DAD_SP() {
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
