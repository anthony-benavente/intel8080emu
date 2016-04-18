#include "cpu/intel8080.hpp"

#include <iostream>
#include <ctime>
using std::cout;
using std::cin;
using std::endl;

#define B 0
#define C 1
#define D 2
#define E 3
#define H 4
#define L 5
#define M 6
#define A 7

// #define VERBOSE

#define STATUS_CARRY 		0x01
#define STATUS_PARITY 		0x04
#define STATUS_AUX_CARRY 	0x10
#define STATUS_SIGN 		0x80
#define STATUS_ZERO 		0x40

void swap(uint8_t *, int, int);

void logstr(const char *str) {
#ifdef VERBOSE
    printf("%s", str);
#endif
}

int num_instr = 0;
void Intel8080::printFlags() {
#ifdef VERBOSE
    printf("%c%c%c%c%c", 
        getFlag(STATUS_SIGN) ? 'S' : ' ', 
        getFlag(STATUS_AUX_CARRY) ? 'A' : ' ',
        getFlag(STATUS_PARITY) ? 'P' : ' ',
        getFlag(STATUS_CARRY) ? 'C' : ' ',
        getFlag(STATUS_ZERO) ? 'Z' : ' ');
#endif
}

void Intel8080::print_status() {
#ifdef VERBOSE
    printf("Num: %d\n", num_instr++);
    printf("Registers:\n");
    printf("A: %2x | F: %2x | B: %2x | C: %2x | D: %2x | E: %2x | H: %2x | L: %2x\n",
        reg[A], status, reg[B], reg[C], reg[D], reg[E], reg[H], reg[L]);
    printf("PC: %x\n", pc);
    printf("SP: %x\n", sp);
    printf("FLAGS: ");
    printFlags();
    printf("\nCYCLES: %d\n", cycles);
#endif
}

void swap(uint8_t *arr, int a, int b) {
	uint8_t tmp = *(arr + a);
	*(arr + a) = *(arr + b);
	*(arr + b) = tmp;
} 

void Intel8080::loadProgram(program_t *program) {
	for (int i = 0; i < program->size; i++) {
		memory[i] = program->data[i];
	}
}

void Intel8080::emulateCycle() {
    print_status();
    
	uint8_t op = getNextOp();
    decode(op);
    
    if (time(NULL) - lastInterrupt > 1.0 / 60) {
        if (inte) {
            handleInterrupt(interruptPin);
            lastInterrupt = time(NULL);
        }
    }
}

unsigned int Intel8080::getPixel(int x, int y) {
    // int byte = memory[0x2400 + ( y * 224 + x)];
    // return byte;
    int tmpX = x;
    x = y;
    y = 255 - tmpX;
    
    int byte = memory[0x2400 + (y * (256 / 8) + (x / 8))];  
    return (byte & (1 << (7 - (x % 8)))) > 0 ? 0xffffff : 0x000000;
}

uint8_t Intel8080::getNextOp() {
	// printf("Returning memory @ %d => 0x%x\n", pc, memory[pc]);
	return memory[pc++];
}

void Intel8080::decode(uint8_t op) {
	uint8_t next = memory[pc];
	uint16_t nextTwo = (next << 8) | memory[pc + 1];
	switch (op) {
		case 0x00: case 0x10: case 0x20: case 0x30:
		case 0x08: case 0x18: case 0x28: case 0x38: NOP(); break;

		case 0x01: pc++; pc++; LXI_r(B, nextTwo); break;
		case 0x11: pc++; pc++; LXI_r(D, nextTwo); break;
		case 0x21: pc++; pc++; LXI_r(H, nextTwo); break;
		case 0x31: pc++; pc++; LXI_SP(nextTwo); break;

		case 0x02: STAX(B); break;
		case 0x12: STAX(D); break;
		case 0x22: pc++; pc++; SHLD(nextTwo); break;
		case 0x32: pc++; pc++; STA(nextTwo); break;

		case 0x03: INX_r(B); break;
		case 0x13: INX_r(D); break;
		case 0x23: INX_r(H); break;
		case 0x33: INX_SP(); break;

		case 0x04: case 0x14: case 0x24: case 0x0c:
		case 0x1c: case 0x2c: case 0x3c: // INR_r
			INR_r(getReg((op & 0x38) >> 3));
			break;
		case 0x34:
			INR_m();
			break;

		case 0x05: case 0x15: case 0x25: case 0x0d:
		case 0x1d: case 0x2d: case 0x3d: // DCR_r
			DCR_r(getReg((op & 0x38) >> 3));
			break;
		case 0x35: DCR_m(); break;

		case 0x06: case 0x16: case 0x26: case 0xe:
		case 0x1e: case 0x2e: case 0x3e: // MVI_r
			pc++;
			MVI_r(getReg((op & 0x38) >> 3), next);
			break;
		case 0x36: pc++; MVI_m(next); break;

		case 0x07: RLC(); break;
		case 0x17: RAL(); break;
		case 0x27: DAA(); break;
		case 0x37: STC(); break;

		case 0x09: case 0x19: case 0x29: // DAD_r
			DAD_r(getRegPair((op & 0x30) >> 4));
			break;
		case 0x39: DAD_SP(); break;

		case 0x0a: LDAX(B); break;
		case 0x1a: LDAX(D);	break;
		case 0x2a: pc++; pc++; LHLD(nextTwo); break;
		case 0x3a: pc++; pc++; LDA(nextTwo); break;

		case 0x0b: case 0x1b: case 0x2b: // DCX_r
			DCX_r(getRegPair((op & 30) >> 4));
			break;
		case 0x3b: DCX_SP(); break;

		case 0x0f: RRC(); break;
		case 0x1f: RAR(); break;
		case 0x2f: CMA(); break;
		case 0x3f: CMC(); break;

		case 0x40: case 0x41: case 0x42: case 0x43:
		case 0x44: case 0x45: case 0x47:
		case 0x48: case 0x49: case 0x4a: case 0x4b:
		case 0x4c: case 0x4d: case 0x4f:
		case 0x50: case 0x51: case 0x52: case 0x53:
		case 0x54: case 0x55: case 0x57:
		case 0x58: case 0x59: case 0x5a: case 0x5b:
		case 0x5c: case 0x5d: case 0x5f:
		case 0x60: case 0x61: case 0x62: case 0x63:
		case 0x64: case 0x65: case 0x67:
		case 0x68: case 0x69: case 0x6a: case 0x6b:
		case 0x6c: case 0x6d: case 0x6f:
		case 0x78: case 0x79: case 0x7a: case 0x7b:
		case 0x7c: case 0x7d: case 0x7f: // MOV_r_r
			MOV_r(getReg(op & 0x7), getReg((op & 0x38) >> 3));
			break;
		case 0x46: case 0x56: case 0x66: case 0x4e:
		case 0x5e: case 0x6e: case 0x7e: // MOV_m_r
			MOV_m_r(getReg(op & 0x7));
			break;
		case 0x70: case 0x71: case 0x72: case 0x73:
		case 0x74: case 0x75: case 0x77: // MOV_r_m
			MOV_r_m(getReg((op & 0x38) >> 3));
			break;

		case 0x80: case 0x81: case 0x82: case 0x83:
		case 0x84: case 0x85: case 0x87: // ADD_r
			ADD_r(getReg(op & 0x7));
			break;
		case 0x86:
			ADD_m();
			break;

		case 0x88: case 0x89: case 0x8a: case 0x8b:
		case 0x8c: case 0x8d: case 0x8f: // ADC_r
			ADC_r(getReg(op & 0x7));
		 	break;
		case 0x8e:
			ADC_m();
			break;

		case 0x90: case 0x91: case 0x92: case 0x93:
		case 0x94: case 0x95: case 0x97: // ADD_r
			SUB_r(getReg(op & 0x7));
			break;
		case 0x96:
			SUB_m();
			break;

		case 0x98: case 0x99: case 0x9a: case 0x9b:
		case 0x9c: case 0x9d: case 0x9f: // ADC_r
			SBB_r(getReg(op & 0x7));
		 	break;
		case 0x9e:
			SBB_m();
			break;

		case 0xa0: case 0xa1: case 0xa2: case 0xa3:
		case 0xa4: case 0xa5: case 0xa7: // ADD_r
			ANA_r(getReg(op & 0x7));
			break;
		case 0xa6:
			ANA_m();
			break;

		case 0xa8: case 0xa9: case 0xaa: case 0xab:
		case 0xac: case 0xad: case 0xaf: // ADC_r
			XRA_r(getReg(op & 0x7));
		 	break;
		case 0xae:
			XRA_m();
			break;

		case 0xb0: case 0xb1: case 0xb2: case 0xb3:
		case 0xb4: case 0xb5: case 0xb7: // ADD_r
			ORA_r(getReg(op & 0x7));
			break;
		case 0xb6:
			ORA_m();
			break;

		case 0xb8: case 0xb9: case 0xba: case 0xbb:
		case 0xbc: case 0xbd: case 0xbf: // ADC_r
			CMP_r(getReg(op & 0x7));
		 	break;
		case 0xbe:
			CMP_m();
			break;

		case 0xc0: RNZ(); break;
		case 0xd0: RNC(); break;
		case 0xe0: RPO(); break;
		case 0xf0: RP(); break;

		case 0xc1: case 0xd1: case 0xe1: case 0xf1:
			POP(getRegPair((op & 0x30) >> 4));
			break;

		case 0xc2: pc++; pc++; JNZ(nextTwo); break;
		case 0xd2: pc++; pc++; JNC(nextTwo); break;
		case 0xe2: pc++; pc++; JPO(nextTwo); break;
		case 0xf2: pc++; pc++; JP(nextTwo); break;

		case 0xc3: case 0xcb: pc++; pc++; JMP(nextTwo); break;
		case 0xd3: pc++; OUT(next); break;
		case 0xe3: XTHL(); break;
		case 0xf3: DI(); break;

		case 0xc4: pc++; pc++; CNZ(nextTwo); break;
		case 0xd4: pc++; pc++; CNC(nextTwo); break;
		case 0xe4: pc++; pc++; CPO(nextTwo); break;
		case 0xf4: pc++; pc++; CP(nextTwo); break;

		case 0xc5: case 0xd5: case 0xe5: case 0xf5:
			PUSH(getRegPair((op & 0x30) >> 4));
			break;

		case 0xc6: pc++; ADI(next); break;
		case 0xd6: pc++; SUI(next); break;
		case 0xe6: pc++; ANI(next); break;
		case 0xf6: pc++; ORI(next); break;

		case 0xc7: case 0xcf:
		case 0xd7: case 0xdf:
		case 0xe7: case 0xef:
		case 0xf7: case 0xff: // RST
			RST((op & 0x38) >> 3);
			break;

		case 0xc8: RZ(); break;
		case 0xd8: RC(); break;
		case 0xe8: RPE(); break;
		case 0xf8: RM(); break;

		case 0xc9: case 0xd9: RET(); break;
		case 0xe9: PCHL(); break;
		case 0xf9: SPHL(); break;

		case 0xca: pc++; pc++; JZ(nextTwo); break;
		case 0xda: pc++; pc++; JC(nextTwo); break;
		case 0xea: pc++; pc++; JPE(nextTwo); break;
		case 0xfa: pc++; pc++; JM(nextTwo); break;

		case 0xdb: pc++; IN(next); break;
		case 0xeb: XCHG(); break;
		case 0xfb: EI(); break;

		case 0xcc: pc++; pc++; CZ(nextTwo); break;
		case 0xdc: pc++; pc++; CC(nextTwo); break;
		case 0xec: pc++; pc++; CPE(nextTwo); break;
		case 0xfc: pc++; pc++; CM(nextTwo); break;

		case 0xcd: case 0xdd: case 0xed: case 0xfd: pc++; pc++; CALL(nextTwo); break;

		case 0xce: pc++; ACI(next); break;
		case 0xde: pc++; SBI(next); break;
		case 0xee: pc++; XRI(next); break;
		case 0xfe: pc++; CPI(next); break;

		default:
			fprintf(stderr, "Invalid op code: 0x%x", op);
			break;
	}
}

void Intel8080::RST(int val) {
	memory[--sp] = pc;
	pc = val;
}

// MOVE STORE LOAD instructions
void Intel8080::MOV_r(uint8_t from, uint8_t to) {
	uint8_t tmp = reg[to];
	reg[to] = reg[from];
	cycles += 5;
}
void Intel8080::MOV_r_m(uint8_t from) {
	uint16_t memIndex = (reg[H] << 8) | reg[L];
	memory[memIndex] = reg[from];
	cycles += 7;
}
void Intel8080::MOV_m_r(uint8_t to) {
	uint16_t memIndex = (reg[H] << 8) | reg[L];
	reg[to] = memory[memIndex];
	cycles += 7;
}
void Intel8080::MVI_m(uint8_t data) {
	uint16_t memIndex = (reg[H] << 8) | reg[L];
	memory[memIndex] = data;
	cycles += 7;
}
void Intel8080::MVI_r(uint8_t from, uint8_t data) {
	reg[from] = data;
	cycles += 7;
}
void Intel8080::LXI_r(uint8_t pair, uint16_t data) {
    // logstr("Running LXI_r\n");
	if (pair == B || pair == D || pair == H) {
		reg[pair] = data & 0xff;
		reg[pair + 1] = (data >> 8) & 0xff;
	}
	cycles += 10;
}
void Intel8080::STAX(uint8_t pair) {
	if (pair == B || pair == D) {
		uint16_t memIndex = (reg[pair] << 8) | reg[pair + 1];
		memory[memIndex] = reg[A];
	}
	cycles += 7;
}
void Intel8080::LDAX(uint8_t pair) {
	if (pair == B || pair == D) {
		uint16_t memIndex = (reg[pair] << 8) | reg[pair + 1];
		reg[A] = memory[memIndex];
	}
	cycles += 7;
}
void Intel8080::STA(uint16_t data) {
	uint8_t low = (data >> 8) & 0xff;
	uint8_t high = data & 0xff;
	uint16_t memIndex = (low << 8) | high;
	memory[memIndex] = reg[A];
	cycles += 13;
}
void Intel8080::LDA(uint16_t data) {
	reg[A] = memory[data];
	cycles += 13;
}
void Intel8080::SHLD(uint16_t data) {
	memory[data] = reg[L];
	memory[data + 1] = reg[H];
	cycles += 16;
}
void Intel8080::LHLD(uint16_t data) {
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
void Intel8080::PUSH(uint8_t pair) {
	if (pair == B || pair == H || pair == D) {
		memory[--sp] = reg[pair];
		memory[--sp] = reg[pair + 1];
	} else if (pair == A) {
		memory[--sp] = reg[A];
		memory[--sp] = status;
	}
	cycles += 11;
}
void Intel8080::POP(uint8_t pair) {
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
	uint8_t tmp = memory[sp];
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
void Intel8080::LXI_SP(uint16_t data) {
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
void Intel8080::JMP(uint16_t data) {
	pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JC(uint16_t data) {
	if (status & STATUS_CARRY)
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JNC(uint16_t data) {
	if (!(status & STATUS_CARRY))
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JZ(uint16_t data) {
	if (status & STATUS_ZERO)
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JNZ(uint16_t data) {
	if (!(status & STATUS_ZERO))
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JP(uint16_t data) {
	if (!(status & STATUS_SIGN))
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JM(uint16_t data) {
	if (status & STATUS_SIGN)
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JPE(uint16_t data) {
	if (status & STATUS_PARITY)
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::JPO(uint16_t data) {
	if (!(status & STATUS_PARITY))
		pc = ((data & 0xff) << 8) | ((data >> 8) & 0xff);
	cycles += 10;
}
void Intel8080::PCHL() {
	pc = (reg[H] << 8) | reg[L];
	cycles += 5;
}

// Call instructions
void Intel8080::CALL(uint16_t data) {
	memory[--sp] = (pc >> 8) & 0xff;
	memory[--sp] = pc & 0xff;
	pc = ((data & 0xff) << 8) | ((data & 0xff00) >> 8);
	cycles += 17;
}
void Intel8080::CC(uint16_t data) {
	if (getFlag(STATUS_CARRY)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CNC(uint16_t data) {
	if (!getFlag(STATUS_CARRY)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CZ(uint16_t data) {
	if (getFlag(STATUS_ZERO)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CNZ(uint16_t data) {
	if (!getFlag(STATUS_ZERO)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CP(uint16_t data) {
	if (!getFlag(STATUS_SIGN)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CM(uint16_t data) {
	if (getFlag(STATUS_SIGN)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CPE(uint16_t data) {
	if (getFlag(STATUS_PARITY)){
		CALL(data);
	} else {
		cycles += 11;
	}
}
void Intel8080::CPO(uint16_t data) {
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
void Intel8080::INR_r(uint8_t regi) {
	reg[regi]++;
	cycles += 5;
}
void Intel8080::INR_m() {
	memory[getHL()]++;
	cycles += 10;
}
void Intel8080::DCR_r(uint8_t regi) {
	reg[regi]--;
    setFlag(STATUS_ZERO, reg[regi] == 0);
    setFlag(STATUS_SIGN, reg[regi] >= 0x80);
    setFlag(STATUS_AUX_CARRY, reg[regi] > 0xf);
    setFlag(STATUS_PARITY, ((reg[regi]) & 1)== 0);
	cycles += 5;
}
void Intel8080::DCR_m() {
	memory[getHL()]--;
    setFlag(STATUS_ZERO, memory[getHL()] == 0);
    setFlag(STATUS_SIGN, memory[getHL()] < 0);
    setFlag(STATUS_AUX_CARRY, memory[getHL()] > 0xf);
    setFlag(STATUS_PARITY, (memory[getHL()] & 1)== 0);
	cycles += 10;
}
void Intel8080::INX_r(uint8_t pair) {
	uint16_t num = (reg[pair] << 8) | reg[pair + 1];
	num++;
	reg[pair] = (num >> 8) & 0xff;
	reg[pair + 1] = num & 0xff;
	cycles += 5;
}
void Intel8080::DCX_r(uint8_t pair) {
	uint16_t num = (reg[pair] << 8) | reg[pair + 1];
	num--;
	reg[pair] = (num >> 8) & 0xff;
	reg[pair + 1] = num & 0xff;
	cycles += 5;
}

// Add instructions
void Intel8080::performAdd(uint8_t data) {
	resetFlags();
	setFlag(STATUS_CARRY, reg[A] + data > 0xff);
	setFlag(STATUS_AUX_CARRY, reg[A] + data > 0xf);
	setFlag(STATUS_ZERO, ((reg[A] + data) & 0xff) == 0);
	setFlag(STATUS_PARITY, ((reg[A] + data) & 0x1) == 0);
	setFlag(STATUS_SIGN, ((reg[A] + data) & 0x80) >> 7);
	reg[A] += data;
}
void Intel8080::ADD_r(uint8_t regi) {
	performAdd(reg[regi]);
	cycles += 4;
}
void Intel8080::ADD_m() {
	performAdd(memory[getHL()]);
	cycles += 7;
}
void Intel8080::ADC_r(uint8_t regi) {
	performAdd(reg[regi] + getFlag(STATUS_CARRY));
	cycles += 4;
}
void Intel8080::ADC_m() {
	performAdd(memory[getHL()] + getFlag(STATUS_CARRY));
	cycles += 7;
}
void Intel8080::ADI(uint8_t data) {
	performAdd(data);
	cycles += 7;
}
void Intel8080::ACI(uint8_t data) {
	performAdd(data + getFlag(STATUS_CARRY));
	cycles += 7;
}
void Intel8080::performDAD(uint16_t data) {
	uint16_t hl = getHL();
	setFlag(STATUS_CARRY, hl + data > 0xffff);
	hl += data;
	reg[H] = (hl >> 8) & 0xff;
	reg[L] = hl & 0xff;
}
void Intel8080::DAD_r(uint8_t pair) {
	performDAD((reg[pair] << 8) | reg[pair + 1]);
	cycles += 10;
}
void Intel8080::DAD_SP() {
	performDAD(sp);
	cycles += 10;
}

// Subtract instructions
void Intel8080::performSub(uint8_t data) {
	resetFlags();
	setFlag(STATUS_CARRY, reg[A] - data < 0);
	setFlag(STATUS_AUX_CARRY, (uint8_t) (reg[A] - data) > 0xf); // TODO: Possible bug here
	setFlag(STATUS_ZERO, ((reg[A] - data) & 0xff) == 0);
	setFlag(STATUS_PARITY, ((reg[A] - data) & 0x1) == 0);
	setFlag(STATUS_SIGN, ((reg[A] - data) & 0x80) >> 7);
	reg[A] -= data;
}
void Intel8080::SUB_r(uint8_t regi) {
	performSub(reg[regi]);
	cycles += 4;
}
void Intel8080::SUB_m() {
	performSub(memory[getHL()]);
	cycles += 7;
}
void Intel8080::SBB_r(uint8_t regi) {
	performSub(reg[regi] + getFlag(STATUS_CARRY));
	cycles += 4;
}
void Intel8080::SBB_m() {
	performSub(memory[getHL()] + getFlag(STATUS_CARRY));
	cycles += 7;
}
void Intel8080::SBI(uint8_t data) {
	performSub(data + getFlag(STATUS_CARRY));
	cycles += 7;
}
void Intel8080::SUI(uint8_t data) {
	performSub(data);
	cycles += 7;
}

// Logical instructions
void Intel8080::performAND(uint8_t data) {
	setFlag(STATUS_CARRY, 0);
	setFlag(STATUS_AUX_CARRY, 0); // TODO: Possible bug. Is aux carry reset?
	reg[A] &= data;
	setFlag(STATUS_ZERO, reg[A] == 0);
	setFlag(STATUS_SIGN, reg[A] >> 7);
	setFlag(STATUS_PARITY, (reg[A] & 0x1) == 0);
}
void Intel8080::performXOR(uint8_t data) {
	setFlag(STATUS_CARRY, 0);
	setFlag(STATUS_AUX_CARRY, 0); // TODO: Possible bug. Is aux carry reset?
	reg[A] ^= data;
	setFlag(STATUS_ZERO, reg[A] == 0);
	setFlag(STATUS_SIGN, reg[A] >> 7);
	setFlag(STATUS_PARITY, (reg[A] & 0x1) == 0);
}
void Intel8080::performOR(uint8_t data) {
	setFlag(STATUS_CARRY, 0);
	setFlag(STATUS_AUX_CARRY, 0); // TODO: Possible bug. Is aux carry reset?
	reg[A] |= data;
	setFlag(STATUS_ZERO, reg[A] == 0);
	setFlag(STATUS_SIGN, reg[A] >> 7);
	setFlag(STATUS_PARITY, (reg[A] & 0x1) == 0);
}
void Intel8080::performCMP(uint8_t data) {
	//resetFlags();
	setFlag(STATUS_CARRY, reg[A] - data < 0);
	setFlag(STATUS_AUX_CARRY, (uint8_t) (reg[A] - data) > 0xf); // TODO: Possible bug here
	setFlag(STATUS_ZERO, ((reg[A] - data) & 0xff) == 0);
	setFlag(STATUS_PARITY, ((reg[A] - data) & 0x1) == 0);
	setFlag(STATUS_SIGN, ((reg[A] - data) & 0x80) >> 7);
}
void Intel8080::ANA_r(uint8_t regi) {
	performAND(reg[regi]);
	cycles += 4;
}
void Intel8080::ANA_m() {
	performAND(memory[getHL()]);
	cycles += 7;
}
void Intel8080::XRA_r(uint8_t regi) {
	performXOR(reg[regi]);
	cycles += 4;
}
void Intel8080::XRA_m() {
	performXOR(memory[getHL()]);
	cycles += 7;
}
void Intel8080::ORA_r(uint8_t regi) {
	performOR(reg[regi]);
	cycles += 4;
}
void Intel8080::ORA_m() {
	performOR(memory[getHL()]);
	cycles += 7;
}
void Intel8080::CMP_r(uint8_t regi) {
	performCMP(reg[regi]);
	cycles += 4;
}
void Intel8080::CMP_m() {
	performCMP(memory[getHL()]);
	cycles += 7;
}
void Intel8080::ANI(uint8_t data) {
	performAND(data);
	cycles += 7;
}
void Intel8080::XRI(uint8_t data) {
	performXOR(data);
	cycles += 7;
}
void Intel8080::ORI(uint8_t data) {
	performOR(data);
	cycles += 7;
}
void Intel8080::CPI(uint8_t data) {
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
	reg[A] = ~reg[A];
	cycles += 4;
}
void Intel8080::STC() {
	setFlag(STATUS_CARRY, 1);
	cycles += 4;
}
void Intel8080::CMC() {
	setFlag(STATUS_CARRY, !getFlag(STATUS_CARRY));
	cycles += 4;
}
void Intel8080::DAA() {
	uint8_t lowNibble = reg[A] & 0xf;

	if (lowNibble > 9 || getFlag(STATUS_AUX_CARRY)) {
		setFlag(STATUS_AUX_CARRY, lowNibble + 6 > 0xf);
		reg[A] += 6;
	}
	uint8_t upNibble = (reg[A] & 0xf0) >> 4;
	if (upNibble > 9) {
		upNibble += 6;
		setFlag(STATUS_CARRY, upNibble > 0xf);
		upNibble &= 0xf;
		reg[A] &= 0xf;
		reg[A] |= (upNibble << 4);
	}
	setFlag(STATUS_ZERO, reg[A] == 0);
	setFlag(STATUS_PARITY, !(reg[A] & 0x1));
	setFlag(STATUS_SIGN, reg[A] >> 7);
	cycles += 4;
}

// Input/Output Instructions
void Intel8080::IN(uint8_t data) {
    reg[A] = inputs[data];
}
void Intel8080::OUT(uint8_t data) {
    outputs[data] = reg[A];
}

// Control Instructions
void Intel8080::EI() {
	inte = true;
	cycles += 4;
}
void Intel8080::DI() {
	inte = false;
	cycles += 4;
}
void Intel8080::NOP() {
	cycles += 4;
}
void Intel8080::HLT() {
	halt = true;
	cycles += 7;
}

void Intel8080::setFlag(int mask, int val) {
	status = val ? status | mask : status & ~mask;
}
int Intel8080::getFlag(int mask) {
	// !! to turn into a 1 or 0 (hackish I know)
	return !!(status & mask);
}
uint16_t Intel8080::getHL() {
	return (reg[H] << 8) | reg[L];
}
void Intel8080::resetFlags() {
	status = 0x0;
}
uint8_t Intel8080::getReg(uint8_t val) {
	return val == 0x0 ? B :
		   val == 0x1 ? C :
		   val == 0x2 ? D :
		   val == 0x3 ? E :
		   val == 0x4 ? H :
		   val == 0x5 ? L : A;
}
uint8_t Intel8080::getRegPair(uint8_t val) {
	return val == 0x0 ? B :
		   val == 0x1 ? D :
		   val == 0x2 ? H : A;
}
void Intel8080::handleInterrupt(uint8_t op) {
    memory[--sp] = pc;
    decode(op);
    pc = memory[sp++];
}