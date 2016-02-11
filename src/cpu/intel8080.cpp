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
	memory[--sp] = pc;
	pc = val;
}

// MOVE STORE LOAD instructions
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

// Stack functions
void Intel8080::PUSH(uint8 pair) {
	if (pair == B || pair == H || pair == D) {
		memory[--sp] = reg[pair];
		memory[--sp] = reg[pair + 1];
	} else if (pair == A) {
		memory[--sp] = reg[A];
		memory[--sp] = status;
	}
	cycles += 11;
}
void Intel8080::POP(uint8 pair) {
	if (pair == B || pair == H || pair == D) {
		reg[pair + 1] = memory[sp++];
		reg[pair] = memory[sp++];
	} else if (pair == A) {
		status = memory[sp++];
		reg[A] = memory[sp++];
	}
	cycles += 10;
}
void Intel8080::XTHL() {
	uint8 tmp = memory[sp];
	memory[sp] = reg[L];
	reg[L] = tmp;

	tmp = memory[sp + 1];
	memory[sp + 1] = reg[H];
	reg[H] = tmp;

	cycles += 18;
}
void Intel8080::SPHL() {
	sp = (reg[H] << 8) | reg[L];
	cycles += 5;
}
void Intel8080::LXI_SP(uint16 data) {
	sp = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::INX_SP() {
	sp++;
	cycles += 5;
}
void Intel8080::DCX_SP() {
	sp--;
	cycles += 5;
}

// JUMP instructions
void Intel8080::JMP(uint16 data) {
	pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JC(uint16 data) {
	if (status & STATUS_CARRY)
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JNC(uint16 data) {
	if (!(status & STATUS_CARRY))
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JZ(uint16 data) {
	if (status & STATUS_ZERO)
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JNZ(uint16 data) {
	if (!(status & STATUS_ZERO))
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JP(uint16 data) {
	if (!(status & STATUS_SIGN))
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JM(uint16 data) {
	if (status & STATUS_SIGN)
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JPE(uint16 data) {
	if (status & STATUS_PARITY)
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JPO(uint16 data) {
	if (!(status & STATUS_PARITY))
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::PCHL() {
	pc = (reg[H] << 8) | reg[L];
	cycles += 5;
}

// Call instructions
void Intel8080::CALL(uint16 data) {
}
void Intel8080::CC(uint16 data) {
}
void Intel8080::CNC(uint16 data) {
}
void Intel8080::CZ(uint16 data) {
}
void Intel8080::CNZ(uint16 data) {
}
void Intel8080::CP(uint16 data) {
}
void Intel8080::CM(uint16 data) {
}
void Intel8080::CPE(uint16 data) {
}
void Intel8080::CPO(uint16 data) {
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
void Intel8080::OUT() {
}
void Intel8080::DI() {
}
void Intel8080::ADI() {
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
void Intel8080::IN() {
}
void Intel8080::EI() {
}
void Intel8080::ACI() {
}
void Intel8080::SBI() {
}
void Intel8080::INX_r(uint8 pair) {
}
void Intel8080::INR_r(uint8 reg) {
}
void Intel8080::INR_m() {
}
void Intel8080::DCR_r(uint8 reg) {
}
void Intel8080::DCR_m() {
}
void Intel8080::DAD_r(uint8 pair) {
}
void Intel8080::ADD(uint8 reg) {
}
void Intel8080::ADC(uint8 reg) {
}
void Intel8080::SUB_r(uint8 reg) {
}
void Intel8080::SUB_m() {
}
void Intel8080::SBB_r(uint8 reg) {
}
void Intel8080::SBB_m() {
}
void Intel8080::ANA_r(uint8 reg) {
}
void Intel8080::ANA_m() {
}
void Intel8080::XRA_r(uint8 reg) {
}
void Intel8080::XRA_m() {
}
void Intel8080::ORA_r(uint8 reg) {
}
void Intel8080::ORA_m() {
}
void Intel8080::CMP_r(uint8 reg) {
}
void Intel8080::CMP_m() {
}
void Intel8080::setFlag(int mask, int val) {
	status = val ? status | mask : status & ~mask;
}
int Intel8080::getFlag(int mask) {
	// !! to turn into a 1 or 0 (hackish I know)
	return !!(status & mask);
}
uint16 Intel8080::getHL() {
	return (reg[H] << 8) | reg[L];
}
