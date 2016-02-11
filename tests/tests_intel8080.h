#include "cxxtest/TestSuite.h"

#include "program/program.hpp"
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

class MemoryTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
		cpu.sp = 0xffff;
	}

	void test_MOV() {
		// Test moving register to register
		TS_ASSERT_EQUALS(cpu.reg[B], 0xb);
		cpu.MOV_r(B, H);
		TS_ASSERT_EQUALS(cpu.reg[B], 0xb);
		TS_ASSERT_EQUALS(cpu.reg[H], 0xb);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);
		cpu.MOV_r(C, B);
		TS_ASSERT_EQUALS(cpu.reg[C], 0xc);
		TS_ASSERT_EQUALS(cpu.reg[B], 0xc);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.MOV_r(L, B);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x17);
		TS_ASSERT_EQUALS(cpu.reg[B], 0x17);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 15);
		cpu.MOV_r(B, C);
		TS_ASSERT_EQUALS(cpu.reg[B], 0x17);
		TS_ASSERT_EQUALS(cpu.reg[C], 0x17);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.MOV_r(C, H);
		TS_ASSERT_EQUALS(cpu.reg[C], 0x17);
		TS_ASSERT_EQUALS(cpu.reg[H], 0x17);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 25);

		// Test moving register to memory
		cpu.MOV_r_m(H);
		TS_ASSERT_EQUALS(cpu.memory[0x1717], 0x17);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 32);
		cpu.MOV_r_m(A);
		TS_ASSERT_EQUALS(cpu.memory[0x1717], 0xa);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 39);

		// Test moving memory to register
		cpu.MOV_m_r(B);
		TS_ASSERT_EQUALS(cpu.reg[B], 0xa)
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 46);
	}

	void test_MVI() {
		// Test moving immediate to memory at H,L pair
		cpu.MVI_m(0xf3);
		TS_ASSERT_EQUALS(cpu.memory[0x1217], 0xf3);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 7);
		cpu.MVI_m(0xdd);
		TS_ASSERT_EQUALS(cpu.memory[0x1217], 0xdd);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 14);

		// Test moving immediate to register
		cpu.MVI_r(B, 0x33);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 21);
		cpu.MVI_r(H, 0x13);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
		cpu.MVI_r(L, 0xd3);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 35);
		TS_ASSERT_EQUALS(cpu.reg[B], 0x33);
		TS_ASSERT_EQUALS(cpu.reg[H], 0x13);
		TS_ASSERT_EQUALS(cpu.reg[L], 0xd3);
	}

	void test_LXI() {
		// Test load immediate to pair B,C
		cpu.LXI_r(B, 0x3219);
		TS_ASSERT_EQUALS(cpu.reg[B], 0x19);
		TS_ASSERT_EQUALS(cpu.reg[C], 0x32);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);

		// Test load immediate to pair D,E
		cpu.LXI_r(D, 0xdead);
		TS_ASSERT_EQUALS(cpu.reg[D], 0xad);
		TS_ASSERT_EQUALS(cpu.reg[E], 0xde);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);

		// Test load immediate to pair H,L
		cpu.LXI_r(H, 0xdad0);
		TS_ASSERT_EQUALS(cpu.reg[H], 0xd0);
		TS_ASSERT_EQUALS(cpu.reg[L], 0xda);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);
	}

	void test_STAX() {
		// Test store accumulator into memory @ B,C
		cpu.STAX(B);
		TS_ASSERT_EQUALS(cpu.memory[0xb0c], 0xa);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 7);

		// Test store accumulator into memory @ D,E
		cpu.STAX(D);
		TS_ASSERT_EQUALS(cpu.memory[0xd0e], 0xa);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 14);
	}

	void test_LDAX() {
		cpu.memory[0xb0c] = 0xba;
		cpu.memory[0xd0e] = 0xbe;

		// Test load accumulator from memory @ B,C
		cpu.LDAX(B);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xba);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 7);

		// Test load accumulator from memory @ D,E
		cpu.LDAX(D);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xbe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 14);
	}

	void test_STA() {
		cpu.STA(0xaaa0);
		TS_ASSERT_EQUALS(cpu.memory[0xaaa0], 0xa);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 13);
	}

	void test_LDA() {
		cpu.memory[0xdead] = 0xba;
		cpu.memory[0xbabe] = 0xbe;

		// Test loading memory @ [pc + 1:pc + 2] into acculumator
		cpu.LDA(0xdead);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xba);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 13);

		cpu.LDA(0xbabe);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xbe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 26);
	}

	void test_SHLD() {
		cpu.SHLD(0xaaaa);
		TS_ASSERT_EQUALS(cpu.memory[0xaaaa], 0x17);
		TS_ASSERT_EQUALS(cpu.memory[0xaaaa + 1], 0x12);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);
	}

	void test_LHLD() {
		cpu.memory[0xdead] = 0xde;
		cpu.memory[0xdead + 1] = 0xad;

		// Load memory @ [pc + 1:pc + 2] -> L, H respectively
		cpu.LHLD(0xdead);
		TS_ASSERT_EQUALS(cpu.reg[L], 0xde);
		TS_ASSERT_EQUALS(cpu.reg[H], 0xad);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);
	}

	void test_XCHG() {
		TS_ASSERT_EQUALS(cpu.reg[D], 0xd);
		TS_ASSERT_EQUALS(cpu.reg[E], 0xe);
		TS_ASSERT_EQUALS(cpu.reg[H], 0x12);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x17);
		// Swap pair D,E with H,L respectively
		cpu.XCHG();
		TS_ASSERT_EQUALS(cpu.reg[D], 0x12);
		TS_ASSERT_EQUALS(cpu.reg[E], 0x17);
		TS_ASSERT_EQUALS(cpu.reg[H], 0xd);
		TS_ASSERT_EQUALS(cpu.reg[L], 0xe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);
	}
};

class StackTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}

	void test_PUSH() {
		uint16 tmpSP = cpu.sp;

		// Test pushing B,C to stack
		cpu.PUSH(B);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0xc);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0xb);
		TS_ASSERT_EQUALS(cpu.sp, tmpSP - 2);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);

		// Test pushing H,L to stack
		cpu.PUSH(H);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x17);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x12);
		TS_ASSERT_EQUALS(cpu.sp, tmpSP - 4);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 22);

		// Test pushing PSW to stack (A, status)
		cpu.PUSH(A);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x0);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0xa);
		TS_ASSERT_EQUALS(cpu.sp, tmpSP - 6);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 33);
	}

	void test_POP() {
		uint16 tmpSP = cpu.sp;
		cpu.memory[--cpu.sp] = 0xde;
		cpu.memory[--cpu.sp] = 0xad;
		cpu.memory[--cpu.sp] = 0xba;
		cpu.memory[--cpu.sp] = 0xbe;
		cpu.memory[--cpu.sp] = 0xda;
		cpu.memory[--cpu.sp] = 0xd0;

		// Test popping from stack into B pair
		cpu.POP(B);
		TS_ASSERT_EQUALS(cpu.reg[B], 0xda);
		TS_ASSERT_EQUALS(cpu.reg[C], 0xd0);
		TS_ASSERT_EQUALS(cpu.sp, tmpSP - 4);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);

		// Test popping from stack into H pair
		cpu.POP(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 0xba);
		TS_ASSERT_EQUALS(cpu.reg[L], 0xbe);
		TS_ASSERT_EQUALS(cpu.sp, tmpSP - 2);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);

		// Test popping from stack into PSW pair
		cpu.POP(A);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xde);
		TS_ASSERT_EQUALS(cpu.status, 0xad);
		TS_ASSERT_EQUALS(cpu.sp, tmpSP);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);
	}

	void test_XTHL() {
		cpu.memory[--cpu.sp] = 0xba;
		cpu.memory[--cpu.sp] = 0xbe;

		// Test exchanging stack[sp, sp+1] with L,H respectively
		cpu.XTHL();
		TS_ASSERT_EQUALS(cpu.reg[L], 0xbe);
		TS_ASSERT_EQUALS(cpu.reg[H], 0xba);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x12);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x17);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 18);
	}

	void test_SPHL() {
		// Test value H << 8 | L getting loaded into stack pointer
		cpu.SPHL();
		TS_ASSERT_EQUALS(cpu.sp, 0x1217);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);
	}

	void test_LXI_SP() {
		// Test sp becomes 3(data) << 8 | 2(data)
		cpu.LXI_SP(0xbabe);
		TS_ASSERT_EQUALS(cpu.sp, 0xbeba);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
	}

	void test_INX_SP() {
		uint16 tmpSP = cpu.sp;

		// Test increase value of SP by 1
		cpu.INX_SP();
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);
		cpu.INX_SP();
		TS_ASSERT_EQUALS(cpu.sp, tmpSP + 2);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
	}

	void test_DCX_SP() {
		uint16 tmpSP = cpu.sp;

		// Test increase value of SP by 1
		cpu.DCX_SP();
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);
		cpu.DCX_SP();
		TS_ASSERT_EQUALS(cpu.sp, tmpSP - 2);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
	}
};

class JumpTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		cpu.pc = 0x0000;
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}

	void test_JMP() {
		// Test setting the program counter to 3(data) << 8 | 2(data)
		cpu.JMP(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0xce00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);

		cpu.JMP(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);

		cpu.JMP(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);
	}

	void test_JC() {
		cpu.JC(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.JC(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.JC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		// Enable carry bit
		cpu.status = (cpu.status | STATUS_CARRY);

		// PC should now be updating
		cpu.JC(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0xce00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
		cpu.JC(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 50);
		cpu.JC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 60);
	}

	void test_JNC() {
		// Enable carry bit
		cpu.status = (cpu.status | STATUS_CARRY);

		// Test JMP called if carry bit one
		cpu.JNC(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.JNC(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.JNC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		// Disable carry bit
		cpu.status = (cpu.status & ~STATUS_CARRY);

		// PC should now be updating
		cpu.JNC(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0xce00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
		cpu.JNC(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 50);
		cpu.JNC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 60);
	}

	void test_JZ() {
		cpu.JZ(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.JZ(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.JZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		// Enable zero bit
		cpu.status = (cpu.status | STATUS_ZERO);

		// PC should now be updating
		cpu.JZ(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0xce00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
		cpu.JZ(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 50);
		cpu.JZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 60);
	}

	void test_JNZ() {
		cpu.status |= STATUS_ZERO;
		// Test JMP called if zero bit 0
		cpu.JNZ(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.JNZ(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.JNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		// Enable zero bit
		cpu.status &= ~STATUS_ZERO;

		// PC should now be updating
		cpu.JNZ(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0xce00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
		cpu.JNZ(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 50);
		cpu.JNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 60);
	}

	void test_JP() {
		cpu.status |= STATUS_SIGN;

		// Test JMP called if zero bit 0
		cpu.JP(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.JP(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.JP(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		// Disable sign bit
		cpu.status &= ~STATUS_SIGN;

		// PC should now be updating
		cpu.JP(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0xce00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
		cpu.JP(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 50);
		cpu.JP(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 60);
	}

	void test_JM() {
		// Test JMP called if zero bit 0
		cpu.JM(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.JM(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.JM(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		// Enable sign bit
		cpu.status |= STATUS_SIGN;

		// PC should now be updating
		cpu.JM(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0xce00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
		cpu.JM(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 50);
		cpu.JM(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 60);
	}

	void test_JPE() {
		// Test JMP called if zero bit 0
		cpu.JPE(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.JPE(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.JPE(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		// Enable sign bit
		cpu.status |= STATUS_PARITY;

		// PC should now be updating
		cpu.JPE(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0xce00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
		cpu.JPE(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 50);
		cpu.JPE(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 60);
	}

	void test_JPO() {
		cpu.status |= STATUS_PARITY;

		// Test JMP called if zero bit 0
		cpu.JPO(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.JPO(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.JPO(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		// Disable sign bit
		cpu.status &= ~STATUS_PARITY;

		// PC should now be updating
		cpu.JPO(0x00ce);
		TS_ASSERT_EQUALS(cpu.pc, 0xce00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
		cpu.JPO(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 50);
		cpu.JPO(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 60);
	}

	void test_PCHL() {
		// Test loading H << 8 | L into program counter
		cpu.PCHL();
	    TS_ASSERT_EQUALS(cpu.pc, 0x1217);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);
	}
};

class CallTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;
	uint16 spTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cpu.sp = 0;
		cyclesTmp = cpu.cycles;
		spTmp = cpu.sp;
	}

	void test_CALL() {
		cpu.CALL(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 2);

		cpu.CALL(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0xbe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0xba);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 34);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 4);

		cpu.CALL(0xc0c0);
		TS_ASSERT_EQUALS(cpu.pc, 0xc0c0);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0xad);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0xde);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 51);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 6);
	}
	void test_CC() {
		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, spTmp);

		cpu.setFlag(STATUS_CARRY, 1);

		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 2);
	}
	void test_CNC() {
		cpu.setFlag(STATUS_CARRY, 1);

		cpu.CNC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, spTmp);

		cpu.setFlag(STATUS_CARRY, 0);

		cpu.CNC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 2);
	}
	void test_CZ() {
		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);

		cpu.setFlag(STATUS_ZERO, 1);

		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 2);
	}
	void test_CNZ() {
		cpu.setFlag(STATUS_ZERO, 1);

		cpu.CNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, spTmp);

		cpu.setFlag(STATUS_ZERO, 0);

		cpu.CNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 2);
	}
	void test_CP() {
		cpu.setFlag(STATUS_SIGN, 1);

		cpu.CNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, spTmp);

		cpu.setFlag(STATUS_SIGN, 0);

		cpu.CNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 2);
	}
	void test_CM() {
		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, spTmp);

		cpu.setFlag(STATUS_SIGN, 1);

		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 2);
	}
	void test_CPE() {
		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, spTmp);

		cpu.setFlag(STATUS_PARITY, 1);

		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 2);
	}
	void test_CPO() {
		cpu.setFlag(STATUS_PARITY, 1);

		cpu.CNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, spTmp);

		cpu.setFlag(STATUS_PARITY, 0);

		cpu.CNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);
		TS_ASSERT_EQUALS(cpu.sp, spTmp - 2);
	}
};

class ReturnTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}

	void test_RET() {
	    TS_ASSERT(false);
	}
	void test_RC() {
	    TS_ASSERT(false);
	}
	void test_RNC() {
	    TS_ASSERT(false);
	}
	void test_RZ() {
	    TS_ASSERT(false);
	}
	void test_RNZ() {
	    TS_ASSERT(false);
	}
	void test_RP() {
	    TS_ASSERT(false);
	}
	void test_RM() {
	    TS_ASSERT(false);
	}
	void test_RPE() {
	    TS_ASSERT(false);
	}
	void test_RPO() {
	    TS_ASSERT(false);
	}
};

class IncrementDecrementTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}
	void test_INR_R() {
		TS_ASSERT(false);
	}
	void test_INR_M() {
	    TS_ASSERT(false);
	}
	void test_DCR_R() {
	    TS_ASSERT(false);
	}
	void test_DCR_M() {
	    TS_ASSERT(false);
	}
	void test_INX_r() {
	    TS_ASSERT(false);
	}
	void test_DCX_r() {
		TS_ASSERT(false);
	}
};

class AddTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}
	void test_ADD_R() {
	    TS_ASSERT(false);
	}
	void test_ADD_M() {
	    TS_ASSERT(false);
	}
	void test_ADC_R() {
	    TS_ASSERT(false);
	}
	void test_ADC_M() {
	    TS_ASSERT(false);
	}
	void test_ADI() {
	    TS_ASSERT(false);
	}
	void test_ACI() {
	    TS_ASSERT(false);
	}
	void test_DAD_B() {
	    TS_ASSERT(false);
	}
	void test_DAD_D() {
	    TS_ASSERT(false);
	}
	void test_DAD_H() {
	    TS_ASSERT(false);
	}
	void test_DAD_SP() {
	    TS_ASSERT(false);
	}
};

class SubtractTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}
	void test_SUB_R() {
	    TS_ASSERT(false);
	}
	void test_SUB_M() {
	    TS_ASSERT(false);
	}
	void test_SBB_R() {
	    TS_ASSERT(false);
	}
	void test_SBB_M() {
	    TS_ASSERT(false);
	}
	void test_SUI() {
		TS_ASSERT(false);
	}
	void test_SBI() {
		TS_ASSERT(false);
	}
};

class LogicalTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}
	void test_ANA_R() {
	    TS_ASSERT(false);
	}
	void test_ANA_M() {
	    TS_ASSERT(false);
	}
	void test_XRA_R() {
	    TS_ASSERT(false);
	}
	void test_XRA_M() {
	    TS_ASSERT(false);
	}
	void test_ORA_R() {
	    TS_ASSERT(false);
	}
	void test_ORA_M() {
	    TS_ASSERT(false);
	}
	void test_CMP_R() {
	    TS_ASSERT(false);
	}
	void test_CMP_M() {
	    TS_ASSERT(false);
	}
	void test_ANI() {
	    TS_ASSERT(false);
	}
	void test_XRI() {
		TS_ASSERT(false);
	}
	void test_ORI() {
	    TS_ASSERT(false);
	}
	void test_CPI() {
		TS_ASSERT(false);
	}
};

class RotateTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}
	void test_RLC() {
	    TS_ASSERT(false);
	}
	void test_RRC() {
	    TS_ASSERT(false);
	}
	void test_RAL() {
	    TS_ASSERT(false);
	}
	void test_RAR() {
	    TS_ASSERT(false);
	}
};

class SpecialsTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}
	void test_CMA() {
		TS_ASSERT(false);
	}
	void test_STC() {
		TS_ASSERT(false);
	}
	void test_CMC() {
		TS_ASSERT(false);
	}
	void test_DAA() {
	    TS_ASSERT(false);
	}
};

class InputOuputTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}
	void test_IN() {
	    TS_ASSERT(false);
	}
	void test_OUT() {
	    TS_ASSERT(false);
	}
};

class ControlTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}

	void test_EI() {
		TS_ASSERT(false);
	}
	void test_DI() {
		TS_ASSERT(false);
	}
	void test_NOP() {
		TS_ASSERT_EQUALS(cpu.sp, 0);
		TS_ASSERT_EQUALS(cpu.pc, 0);
		TS_ASSERT_EQUALS(cpu.inte, false);
		TS_ASSERT_EQUALS(cpu.status, 0);
		for (int i = 0; i < 8; i++) {
			TS_ASSERT_EQUALS(cpu.reg[i], 0);
		}

		cpu.NOP();

		// NOP should have done nothing
		TS_ASSERT_EQUALS(cpu.sp, 0);
		TS_ASSERT_EQUALS(cpu.pc, 0);
		TS_ASSERT_EQUALS(cpu.inte, false);
		TS_ASSERT_EQUALS(cpu.status, 0);
		for (int i = 0; i < 8; i++) {
			TS_ASSERT_EQUALS(cpu.reg[i], 0);
		}
	}
	void test_HLT() {
	    TS_ASSERT(false);
	}
};

class ResetTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}

	void test_RST() {
		cpu.pc = 0;
		cpu.sp = 0;
		for (int i = 1; i < 8; i++) {
			cpu.RST(i);
			TS_ASSERT_EQUALS(cpu.pc, i)
			TS_ASSERT_EQUALS(cpu.memory[cpu.sp++], i - 1);
		}
	}
};

class GeneralTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32 cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0xa;
		cyclesTmp = cpu.cycles;
	}

	void test_getNextOp(void) {
		cpu.loadProgram(getProgram("../res/invaders.rom"));
		cpu.getNextOp();
		cpu.getNextOp();
		cpu.getNextOp();
		TS_ASSERT_EQUALS(cpu.getPc(), 0x03);
		TS_ASSERT_EQUALS(cpu.getNextOp(), 0xc3);
		TS_ASSERT_EQUALS(cpu.getPc(), 0x4);
		TS_ASSERT_EQUALS(cpu.getNextOp(), 0xd4);
		TS_ASSERT_EQUALS(cpu.getPc(), 0x5);
		TS_ASSERT_EQUALS(cpu.getNextOp(), 0x18);
		TS_ASSERT_EQUALS(cpu.getPc(), 0x6);
		TS_ASSERT_EQUALS(cpu.getNextOp(), 0x00);
		TS_ASSERT_EQUALS(cpu.getPc(), 0x7);
		TS_ASSERT_EQUALS(cpu.getNextOp(), 0x00);
		TS_ASSERT_EQUALS(cpu.getPc(), 0x8);
		TS_ASSERT_EQUALS(cpu.getNextOp(), 0xf5);
		for (int i = 0; i <= 0xffff; i++) {
			cpu.memory[i] = 0;
		}
		cpu.pc = 0;
	}

	void test_getFlag(void) {
		cpu.status |= STATUS_ZERO;
		cpu.status |= STATUS_PARITY;
		cpu.status |= STATUS_AUX_CARRY;
		cpu.status |= STATUS_CARRY;
		cpu.status |= STATUS_SIGN;

		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);

		cpu.status &= ~STATUS_ZERO;
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		cpu.status &= ~STATUS_PARITY;
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		cpu.status &= ~STATUS_AUX_CARRY;
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		cpu.status &= ~STATUS_SIGN;
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		cpu.status &= ~STATUS_CARRY;
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
	}

	void test_decode() {
	    TS_ASSERT(false);
	}
};
