#include "cpu/chip8.hpp"

#include "program/program.hpp"

#include <iostream>
#include <stdlib.h>
#include <time.h>

Chip8::Chip8() {
	pc = 0x200;
	sp = 0;
	I = 0;
	op = 0;
	delay = 0;
	sound = 0;
	drawFlag = false;

	for (int i = 0; i < 4096; i++) {
		if (i < 80) {
			memory[i] = fontSet[i];
		} else {
			memory[i] = 0;
		}
	}

	for (int i = 0; i < 16; i++) {
		V[i] = 0;
		stack[i] = 0;
		keys[i] = false;
	}

	srand(time(NULL));
}

Chip8::~Chip8() {}

void Chip8::loadProgram(program_t *program) {
	for (unsigned int i = 0x200; i < 0x200 + program->size; i++) {
		memory[i] = program->data[i - 0x200];
	}
}


void Chip8::emulateCycle() {
	op = Chip8::getNextOp();
	pc += 2;
	opLookUp(op & 0xffff);

	if (delay > 0) --delay;
	if (sound > 0) {
		if (sound-- == 1) printf("Beep here\n");
	}
}

unsigned short Chip8::getNextOp() {
	return  ((memory[pc] << 8) | memory[pc + 1]);
}

unsigned char Chip8::getPixel(int x, int y) {
	return gfx[y * 64 + x];
}

void Chip8::opLookUp(unsigned short op) {
	unsigned int x = (op & 0xf00) >> 8;
	unsigned int y = (op & 0xf0)  >> 4;
	switch ((op & 0xf000) >> 12) {
		case 0x0:
			switch (op & 0x00ff) {
				case 0xee: RET(); break;
				case 0xe0: CLS(); break;
				// default: SYS(op); break;
				default: break;
			}
			break;
		case 0x1:
			JP_addr(op);
			break;
		case 0x2: CALL_addr(op); break;
		case 0x3: SE_Vx_byte(op, x); break;
		case 0x4: SNE_Vx_byte(op, x); break;
		case 0x5: SE_Vx_Vy(op, x, y); break;
		case 0x6: LD_Vx_byte(op, x); break;
		case 0x7: ADD_Vx_byte(op, x); break;
		case 0x8:
			switch (op & 0xf) {
				case 0x0: LD_Vx_Vy(op, x, y); break;
				case 0x1: OR_Vx_Vy(op, x, y); break;
				case 0x2: AND_Vx_Vy(op, x, y); break;
				case 0x3: XOR_Vx_Vy(op, x, y); break;
				case 0x4: ADD_Vx_Vy(op, x, y); break;
				case 0x5: SUB_Vx_Vy(op, x, y); break;
				case 0x6: SHR_Vx(op, x); break;
				case 0x7: SUBN_Vx_Vy(op, x, y); break;
				case 0xE: SHL_Vx(op, x); break;
			}
			break;
		case 0x9: SNE_Vx_Vy(op, x, y); break;
		case 0xA: LD_I_addr(op); break;
		case 0xB: JP_V0_addr(op); break;
		case 0xC: RND_Vx_byte(op, x); break;
		case 0xD: DRW_Vx_Vy_nibble(op, x, y); break;
		case 0xE:
		 	switch (op & 0xff) {
				case 0x9e: SKP_Vx(op, x); break;
				case 0xa1: SKNP_Vx(op, x); break;
			}
			break;
		case 0xF:
			switch (op & 0xff) {
				case 0x07: LD_Vx_DT(op, x); break;
				case 0x0A: LD_Vx_K(op, x); break;
				case 0x15: LD_DT_Vx(op, x); break;
				case 0x18: LD_ST_Vx(op, x); break;
				case 0x1E: ADD_I_Vx(op, x); break;
				case 0x29: LD_F_Vx(op, x); break;
				case 0x33: LD_B_Vx(op, x); break;
				case 0x55: LD_I_Vx(op, x); break;
				case 0x65: LD_Vx_I(op, x); break;
			}
			break;
		default:
			printf("Invalid opcode (0x%x)\n", op);
	}
}

void Chip8::SYS(unsigned short op) {
	pc = op & 0xfff;
}

void Chip8::CLS() {
	for (int i = 0; i < 32 * 64; i++) {
		gfx[i] = 0;
	}
	drawFlag = true;
}

void Chip8::RET() {
	pc = stack[--sp];
	pc += 2;
}

void Chip8::JP_addr(unsigned short op) {
	pc = op & 0xfff;
}

void Chip8::CALL_addr(unsigned short op) {
	stack[sp++] = pc - 2;
	pc = op & 0xfff;
}

void Chip8::SE_Vx_byte(unsigned short op, unsigned char x) {
	if (V[x] == (op & 0xff)) {
		pc += 2;
	}
}

void Chip8::SNE_Vx_byte(unsigned short op, unsigned char x) {
	if (V[x] != (op & 0xff)) {
		pc += 2;
	}
}

void Chip8::SE_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	if (V[x] == V[y]) {
		pc += 2;
	}
}

void Chip8::LD_Vx_byte(unsigned short op, unsigned char x) {
	V[x] = op & 0xff;
}

void Chip8::ADD_Vx_byte(unsigned short op, unsigned char x) {
	V[x] += op & 0xff;
}

void Chip8::LD_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[x] = V[y];
}

void Chip8::OR_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[x] |= V[y];
}

void Chip8::AND_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[x] &= V[y];
}

void Chip8::XOR_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[x] ^= V[y];
}

void Chip8::ADD_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[0xf] = V[x] + V[y] > 0xff ? 1 : 0;
	V[x] = (V[x] + V[y]);
}

void Chip8::SUB_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[0xf] = V[x] > V[y] ? 1 : 0;
	V[x] -= V[y];
}

void Chip8::SHR_Vx(unsigned short op, unsigned char x) {
	V[0xf] = V[x] & 0x1;
	V[x] >>= 1;
}

void Chip8::SUBN_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[0xf] = V[y] > V[x] ? 1 : 0;
	V[x] = V[y] - V[x];
}

void Chip8::SHL_Vx(unsigned short op, unsigned char x) {
	V[0xf] = V[x] >> 7;
	V[x] <<= 1;
}

void Chip8::SNE_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	if (V[x] != V[y]) {
		pc += 2;
	}
}

void Chip8::LD_I_addr(unsigned short op) {
	I = op & 0xfff;
}

void Chip8::JP_V0_addr(unsigned short op) {
	pc = (op & 0xfff) + V[0];
}

void Chip8::RND_Vx_byte(unsigned short op, unsigned char x) {
	V[x] = (rand() % 0xff) & (op & 0xff);
}

void Chip8::DRW_Vx_Vy_nibble(unsigned short op, unsigned char x, unsigned char y) {
	unsigned short xp = V[x];
	unsigned short yp = V[y];
	unsigned short pixel;

	V[0xf] = 0;
	for (int yline = 0; yline < (op & 0xf); yline++) {
		pixel = memory[I + yline];
		for (int xline = 0; xline < 8; xline++) {
			if ((yp + yline) < 32 && (xp + xline) < 64) {
				int idx = (yp + yline) * 64 + (xp + xline);
				if ((pixel & (0x80 >> xline)) != 0) {
					if (gfx[idx]) {
						V[0xf] = 1;
					}
					gfx[idx] ^= 1;
				}
			}
		}
	}

	drawFlag = true;
}

void Chip8::SKP_Vx(unsigned short op, unsigned char x) {
	if (keys[(int) V[x]]) {
		pc += 2;
	}
}

void Chip8::SKNP_Vx(unsigned short op, unsigned char x) {
	if (!keys[(int) V[x]]) {
		pc += 2;
	}
}

void Chip8::LD_Vx_DT(unsigned short op, unsigned char x) {
	V[x] = delay;
}

void Chip8::LD_Vx_K(unsigned short op, unsigned char x) {
	// Wait for keypress
}

void Chip8::LD_DT_Vx(unsigned short op, unsigned char x) {
	delay = V[x];
}

void Chip8::LD_ST_Vx(unsigned short op, unsigned char x) {
	sound = V[x];
}

void Chip8::ADD_I_Vx(unsigned short op, unsigned char x) {
	V[0xf] = I + V[x] > 0xfff; // Why 0xfff?
	I += V[x];
}

void Chip8::LD_F_Vx(unsigned short op, unsigned char x) {
	I = V[x] * 0x5; // Why?
}

void Chip8::LD_B_Vx(unsigned short op, unsigned char x) {
	memory[I] = (V[x] / 100) % 10;
	memory[I + 1] = (V[x] / 10) % 10;
	memory[I + 2] = (V[x] / 1) % 10;
}

void Chip8::LD_I_Vx(unsigned short op, unsigned char x) {
	for (int i = 0; i <= x; i++) {
		memory[I + i] = V[i];
	}

	I += x + 1;
}

void Chip8::LD_Vx_I(unsigned short op, unsigned char x) {
	for (int i = 0; i <= x; i++) {
		V[i] = memory[I + i];
	}

	I += x + 1;
}
