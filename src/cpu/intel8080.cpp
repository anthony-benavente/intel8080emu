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
	memory[--sp] = (pc >> 8) & 0xff;
	memory[--sp] = pc & 0xff;
	pc = ((data & 0xff) << 8) | ((data & 0xff00) >> 8);
	cycles += 17;
}
void Intel8080::CC(uint16 data) {
	if (getFlag(STATUS_CARRY)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CNC(uint16 data) {
	if (!getFlag(STATUS_CARRY)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CZ(uint16 data) {
	if (getFlag(STATUS_ZERO)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CNZ(uint16 data) {
	if (!getFlag(STATUS_ZERO)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CP(uint16 data) {
	if (!getFlag(STATUS_SIGN)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CM(uint16 data) {
	if (getFlag(STATUS_SIGN)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CPE(uint16 data) {
	if (getFlag(STATUS_PARITY)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CPO(uint16 data) {
	if (!getFlag(STATUS_PARITY)){
		CALL(data);
	} else {
		cycles += 11;
	}
}

// Return instructions
void Intel8080::RET() {
	pc = (memory[sp + 1] << 8) | memory[sp];
	sp += 2;
	cycles += 10;
}
void Intel8080::RC() {
	cycles += 1;
	if (getFlag(STATUS_CARRY)) {
		RET();
	} else {
		cycles += 4;
	}
}
void Intel8080::RNC() {
	cycles += 1;
	if (!getFlag(STATUS_CARRY)) {
		RET();
	} else {
		cycles += 4;
	}
}
void Intel8080::RZ() {
	cycles += 1;
	if (getFlag(STATUS_ZERO)) {
		RET();
	} else {
		cycles += 4;
	}
}
void Intel8080::RNZ() {
	cycles += 1;
	if (!getFlag(STATUS_ZERO)) {
		RET();
	} else {
		cycles += 4;
	}
}
void Intel8080::RP() {
	cycles += 1;
	if (!getFlag(STATUS_SIGN)) {
		RET();
	} else {
		cycles += 4;
	}
}
void Intel8080::RM() {
	cycles += 1;
	if (getFlag(STATUS_SIGN)) {
		RET();
	} else {
		cycles += 4;
	}
}
void Intel8080::RPE() {
	cycles += 1;
	if (getFlag(STATUS_PARITY)) {
		RET();
	} else {
		cycles += 4;
	}
}
void Intel8080::RPO() {
	cycles += 1;
	if (!getFlag(STATUS_PARITY)) {
		RET();
	} else {
		cycles += 4;
	}
}

// Increment/Decrement instructions
void Intel8080::INR_r(uint8 regi) {
	reg[regi]++;
	cycles += 5;
}
void Intel8080::INR_m() {
	memory[getHL()]++;
	cycles += 10;
}
void Intel8080::DCR_r(uint8 regi) {
	reg[regi]--;
	cycles += 5;
}
void Intel8080::DCR_m() {
	memory[getHL()]--;
	cycles += 10;
}
void Intel8080::INX_r(uint8 pair) {
	uint16 num = (reg[pair] << 8) | reg[pair + 1];
	num++;
	reg[pair] = (num >> 8) & 0xff;
	reg[pair + 1] = num & 0xff;
	cycles += 5;
}
void Intel8080::DCX_r(uint8 pair) {
	uint16 num = (reg[pair] << 8) | reg[pair + 1];
	num--;
	reg[pair] = (num >> 8) & 0xff;
	reg[pair + 1] = num & 0xff;
	cycles += 5;
}

// Add instructions
void Intel8080::performAdd(uint8 data) {
	resetFlags();
	setFlag(STATUS_CARRY, reg[A] + data > 0xff);
	setFlag(STATUS_AUX_CARRY, reg[A] + data > 0xf);
	setFlag(STATUS_ZERO, ((reg[A] + data) & 0xff) == 0);
	setFlag(STATUS_PARITY, ((reg[A] + data) & 0x1) == 0);
	setFlag(STATUS_SIGN, ((reg[A] + data) & 0x80) >> 7);
	reg[A] += data;
}
void Intel8080::ADD_r(uint8 regi) {
	performAdd(reg[regi]);
	cycles += 4;
}
void Intel8080::ADD_m() {
	performAdd(memory[getHL()]);
	cycles += 7;
}
void Intel8080::ADC_r(uint8 regi) {
	performAdd(reg[regi] + getFlag(STATUS_CARRY));
	cycles += 4;
}
void Intel8080::ADC_m() {
	performAdd(memory[getHL()] + getFlag(STATUS_CARRY));
	cycles += 7;
}
void Intel8080::ADI(uint8 data) {
	performAdd(data);
	cycles += 7;
}
void Intel8080::ACI(uint8 data) {
	performAdd(data + getFlag(STATUS_CARRY));
	cycles += 7;
}
void Intel8080::performDAD(uint16 data) {
	uint16 hl = getHL();
	setFlag(STATUS_CARRY, hl + data > 0xffff);
	hl += data;
	reg[H] = (hl >> 8) & 0xff;
	reg[L] = hl & 0xff;
}
void Intel8080::DAD_r(uint8 pair) {
	performDAD((reg[pair] << 8) | reg[pair + 1]);
	cycles += 10;
}
void Intel8080::DAD_SP() {
	performDAD(sp);
	cycles += 10;
}

// Subtract instructions
void Intel8080::performSub(uint8 data) {
	resetFlags();
	setFlag(STATUS_CARRY, reg[A] - data < 0);
	setFlag(STATUS_AUX_CARRY, (uint8) (reg[A] - data) > 0xf); // TODO: Possible bug here
	setFlag(STATUS_ZERO, ((reg[A] - data) & 0xff) == 0);
	setFlag(STATUS_PARITY, ((reg[A] - data) & 0x1) == 0);
	setFlag(STATUS_SIGN, ((reg[A] - data) & 0x80) >> 7);
	reg[A] -= data;
}
void Intel8080::SUB_r(uint8 regi) {
	performSub(reg[regi]);
	cycles += 4;
}
void Intel8080::SUB_m() {
	performSub(memory[getHL()]);
	cycles += 7;
}
void Intel8080::SBB_r(uint8 regi) {
	performSub(reg[regi] + getFlag(STATUS_CARRY));
	cycles += 4;
}
void Intel8080::SBB_m() {
	performSub(memory[getHL()] + getFlag(STATUS_CARRY));
	cycles += 7;
}
void Intel8080::SBI(uint8 data) {
	performSub(data + getFlag(STATUS_CARRY));
	cycles += 7;
}
void Intel8080::SUI(uint8 data) {
	performSub(data);
	cycles += 7;
}

// Logical instructions
void Intel8080::performAND(uint8 data) {
	setFlag(STATUS_CARRY, 0);
	setFlag(STATUS_AUX_CARRY, 0); // TODO: Possible bug. Is aux carry reset?
	reg[A] &= data;
	setFlag(STATUS_ZERO, reg[A] == 0);
	setFlag(STATUS_SIGN, reg[A] >> 7);
	setFlag(STATUS_PARITY, (reg[A] & 0x1) == 0);
}
void Intel8080::performXOR(uint8 data) {
	setFlag(STATUS_CARRY, 0);
	setFlag(STATUS_AUX_CARRY, 0); // TODO: Possible bug. Is aux carry reset?
	reg[A] ^= data;
	setFlag(STATUS_ZERO, reg[A] == 0);
	setFlag(STATUS_SIGN, reg[A] >> 7);
	setFlag(STATUS_PARITY, (reg[A] & 0x1) == 0);
}
void Intel8080::performOR(uint8 data) {
	setFlag(STATUS_CARRY, 0);
	setFlag(STATUS_AUX_CARRY, 0); // TODO: Possible bug. Is aux carry reset?
	reg[A] |= data;
	setFlag(STATUS_ZERO, reg[A] == 0);
	setFlag(STATUS_SIGN, reg[A] >> 7);
	setFlag(STATUS_PARITY, (reg[A] & 0x1) == 0);
}
void Intel8080::performCMP(uint8 data) {
	resetFlags();
	setFlag(STATUS_CARRY, reg[A] - data < 0);
	setFlag(STATUS_AUX_CARRY, (uint8) (reg[A] - data) > 0xf); // TODO: Possible bug here
	setFlag(STATUS_ZERO, ((reg[A] - data) & 0xff) == 0);
	setFlag(STATUS_PARITY, ((reg[A] - data) & 0x1) == 0);
	setFlag(STATUS_SIGN, ((reg[A] - data) & 0x80) >> 7);
}
void Intel8080::ANA_r(uint8 regi) {
	performAND(reg[regi]);
	cycles += 4;
}
void Intel8080::ANA_m() {
	performAND(memory[getHL()]);
	cycles += 7;
}
void Intel8080::XRA_r(uint8 regi) {
	performXOR(reg[regi]);
	cycles += 4;
}
void Intel8080::XRA_m() {
	performXOR(memory[getHL()]);
	cycles += 7;
}
void Intel8080::ORA_r(uint8 regi) {
	performOR(reg[regi]);
	cycles += 4;
}
void Intel8080::ORA_m() {
	performOR(memory[getHL()]);
	cycles += 7;
}
void Intel8080::CMP_r(uint8 regi) {
	performCMP(reg[regi]);
	cycles += 4;
}
void Intel8080::CMP_m() {
	performCMP(memory[getHL()]);
	cycles += 7;
}
void Intel8080::ANI(uint8 data) {
	performAND(data);
	cycles += 7;
}
void Intel8080::XRI(uint8 data) {
	performXOR(data);
	cycles += 7;
}
void Intel8080::ORI(uint8 data) {
	performOR(data);
	cycles += 7;
}
void Intel8080::CPI(uint8 data) {
	performCMP(data);
	cycles += 7;
}

// Rotate Instructions
void Intel8080::RLC() {
	setFlag(STATUS_CARRY, (reg[A] & 0x80) == 0x80);
	reg[A] <<= 1;
	reg[A] |= getFlag(STATUS_CARRY);
	cycles += 4;
}
void Intel8080::RRC() {
	setFlag(STATUS_CARRY, reg[A] & 0x1);
	reg[A] >>= 1;
	reg[A] |= getFlag(STATUS_CARRY) << 7;
	cycles += 4;
}
void Intel8080::RAL() {
	setFlag(STATUS_CARRY, (reg[A] & 0x80) == 0x80);
	reg[A] <<= 1;
	cycles += 4;
}
void Intel8080::RAR() {
	setFlag(STATUS_CARRY, reg[A] & 0x1);
	reg[A] >>= 1;
	cycles += 4;
}

// Special Instructions
void Intel8080::CMA() {
}
void Intel8080::STC() {
}
void Intel8080::CMC() {
}
void Intel8080::DAA() {
}

// Input/Output Instructions
void Intel8080::IN() {
}
void Intel8080::OUT() {
}

// Control Instructions
void Intel8080::EI() {
}
void Intel8080::DI() {
}
void Intel8080::HLT() {
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
void Intel8080::resetFlags() {
	status = 0xa;
}
