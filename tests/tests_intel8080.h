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
	uint32_t cyclesTmp;

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
	uint32_t cyclesTmp;

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
		uint32_t tmpSP = 0xffff + 1;

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
		uint8_t statusByte = cpu.status;
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], statusByte);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0xa);
		TS_ASSERT_EQUALS(cpu.sp, tmpSP - 6);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 33);
	}

	void test_POP() {
		uint16_t tmpSP = cpu.sp;
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
		uint16_t tmpSP = cpu.sp;

		// Test increase value of SP by 1
		cpu.INX_SP();
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);
		cpu.INX_SP();
		TS_ASSERT_EQUALS(cpu.sp, tmpSP + 2);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
	}

	void test_DCX_SP() {
		uint16_t tmpSP = cpu.sp;

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
	uint32_t cyclesTmp;

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
		cpu.sp = 0x0;
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
		// Set 0 flag to 0 so nothing works
		cpu.setFlag(STATUS_ZERO, 0);

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
	uint32_t cyclesTmp;
	uint16_t spTmp;

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
		cpu.cycles = 0;
		cyclesTmp = cpu.cycles;
		cpu.pc = 0;
		cpu.status = 0;
		cpu.sp = 0;
		spTmp = cpu.sp;
	}

	void test_CALL() {
		cpu.CALL(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 17);

		cpu.CALL(0xadde);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffc);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0xbe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0xba);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 34);

		cpu.CALL(0xc0c0);
		TS_ASSERT_EQUALS(cpu.pc, 0xc0c0);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffa);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0xad);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0xde);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 51);
	}
	void test_CC() {
		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, 0x0);

		cpu.setFlag(STATUS_CARRY, 1);

		cpu.CC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
	}
	void test_CNC() {
		cpu.setFlag(STATUS_CARRY, 1);

		cpu.CNC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, 0x0);

		cpu.setFlag(STATUS_CARRY, 0);

		cpu.CNC(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
	}
	void test_CZ() {
		cpu.CZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, 0x0);

		cpu.setFlag(STATUS_ZERO, 1);

		cpu.CZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
	}
	void test_CNZ() {
		cpu.setFlag(STATUS_ZERO, 1);

		cpu.CNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, 0x0);

		cpu.setFlag(STATUS_ZERO, 0);

		cpu.CNZ(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
	}
	void test_CP() {
		cpu.setFlag(STATUS_SIGN, 1);

		cpu.CP(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, 0x0);

		cpu.setFlag(STATUS_SIGN, 0);

		cpu.CP(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
	}
	void test_CM() {
		cpu.CM(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, 0x0);

		cpu.setFlag(STATUS_SIGN, 1);

		cpu.CM(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
	}
	void test_CPE() {
		cpu.CPE(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, 0x0);

		cpu.setFlag(STATUS_PARITY, 1);

		cpu.CPE(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
	}
	void test_CPO() {
		cpu.setFlag(STATUS_PARITY, 1);

		cpu.CPO(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 11);
		TS_ASSERT_EQUALS(cpu.sp, 0x0);

		cpu.setFlag(STATUS_PARITY, 0);

		cpu.CPO(0xbeba);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[cpu.sp + 1], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
	}
};

class ReturnTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;
	uint16_t spTmp;

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
		cpu.pc = 0;
		cyclesTmp = cpu.cycles;
		cpu.sp = 0xffff + 1;
		spTmp = cpu.sp;
		cpu.memory[--cpu.sp] = 0xba;
		cpu.memory[--cpu.sp] = 0xbe;
		cpu.memory[--cpu.sp] = 0xde;
		cpu.memory[--cpu.sp] = 0xad;
	}

	void test_RET() {
		cpu.RET();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);

		cpu.RET();
		TS_ASSERT_EQUALS(cpu.sp, 0);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
	}

	void test_RC() {
		cpu.RC();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffc);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.setFlag(STATUS_CARRY, 1);
		cpu.RC();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.RC();
		TS_ASSERT_EQUALS(cpu.sp, 0x0);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 27);
	}

	void test_RNC() {
		cpu.setFlag(STATUS_CARRY, 1);
		cpu.RNC();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffc);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.setFlag(STATUS_CARRY, 0);
		cpu.RNC();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.RNC();
		TS_ASSERT_EQUALS(cpu.sp, 0x0);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 27);
	}

	void test_RZ() {
		cpu.setFlag(STATUS_ZERO, 0);

		cpu.RZ();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffc);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.setFlag(STATUS_ZERO, 1);
		cpu.RZ();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.RZ();
		TS_ASSERT_EQUALS(cpu.sp, 0x0);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 27);
	}

	void test_RNZ() {
		cpu.setFlag(STATUS_ZERO, 1);
		cpu.RNZ();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffc);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.setFlag(STATUS_ZERO, 0);
		cpu.RNZ();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.RNZ();
		TS_ASSERT_EQUALS(cpu.sp, 0x0);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 27);
	}

	void test_RP() {
		cpu.setFlag(STATUS_SIGN, 1);
		cpu.RP();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffc);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.setFlag(STATUS_SIGN, 0);
		cpu.RP();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.RP();
		TS_ASSERT_EQUALS(cpu.sp, 0x0);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 27);
	}

	void test_RM() {
		cpu.RM();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffc);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.setFlag(STATUS_SIGN, 1);
		cpu.RM();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.RM();
		TS_ASSERT_EQUALS(cpu.sp, 0x0);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 27);
	}

	void test_RPE() {
		cpu.RPE();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffc);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.setFlag(STATUS_PARITY, 1);
		cpu.RPE();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.RPE();
		TS_ASSERT_EQUALS(cpu.sp, 0x0);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 27);
	}

	void test_RPO() {
		cpu.setFlag(STATUS_PARITY, 1);
		cpu.RPO();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffc);
		TS_ASSERT_EQUALS(cpu.pc, 0x0000);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.setFlag(STATUS_PARITY, 0);
		cpu.RPO();
		TS_ASSERT_EQUALS(cpu.sp, 0xfffe);
		TS_ASSERT_EQUALS(cpu.pc, 0xdead);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.RPO();
		TS_ASSERT_EQUALS(cpu.sp, 0x0);
		TS_ASSERT_EQUALS(cpu.pc, 0xbabe);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 27);
	}
};

class IncrementDecrementTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0x0;
		cpu.reg[C] = 0x0;
		cpu.reg[D] = 0x0;
		cpu.reg[E] = 0x0;
		cpu.reg[H] = 0x0;
		cpu.reg[L] = 0x0;
		cpu.reg[A] = 0x0;
		cpu.pc = 0;
		cpu.cycles = 0;
		cyclesTmp = cpu.cycles;
	}
	void test_INR_r() {
		cpu.INR_r(B);
		TS_ASSERT_EQUALS(cpu.reg[B], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.INR_r(C);
		TS_ASSERT_EQUALS(cpu.reg[C], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);

		cpu.INR_r(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 15);

		cpu.INR_r(L);
		TS_ASSERT_EQUALS(cpu.reg[L], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);

		cpu.INR_r(D);
		TS_ASSERT_EQUALS(cpu.reg[D], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 25);

		cpu.INR_r(E);
		TS_ASSERT_EQUALS(cpu.reg[E], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		cpu.INR_r(A);
		TS_ASSERT_EQUALS(cpu.reg[A], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 35);
	}

	void test_INR_m() {
		cpu.reg[H] = 0xba;
		cpu.reg[L] = 0xbe;
		cpu.INR_m();
		TS_ASSERT_EQUALS(cpu.memory[cpu.getHL()], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);

		cpu.INR_m();
		TS_ASSERT_EQUALS(cpu.memory[cpu.getHL()], 2);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);

		cpu.reg[H] = 0xde;
		cpu.reg[L] = 0xad;
		cpu.INR_m();
		TS_ASSERT_EQUALS(cpu.memory[cpu.getHL()], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		cpu.INR_m();
		TS_ASSERT_EQUALS(cpu.memory[cpu.getHL()], 2);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
	}

	void test_DCR_r() {
		cpu.INR_r(B);
		cpu.INR_r(B);
		cpu.INR_r(C);
		cpu.INR_r(C);
		cpu.INR_r(D);
		cpu.INR_r(D);
		cpu.INR_r(E);
		cpu.INR_r(E);
		cpu.INR_r(H);
		cpu.INR_r(H);
		cpu.INR_r(L);
		cpu.INR_r(L);
		cpu.INR_r(A);
		cpu.INR_r(A);
		cpu.cycles = 0;

		cpu.DCR_r(B);
		TS_ASSERT_EQUALS(cpu.reg[B], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);

		cpu.DCR_r(C);
		TS_ASSERT_EQUALS(cpu.reg[C], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);

		cpu.DCR_r(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 15);

		cpu.DCR_r(L);
		TS_ASSERT_EQUALS(cpu.reg[L], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);

		cpu.DCR_r(D);
		TS_ASSERT_EQUALS(cpu.reg[D], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 25);

		cpu.DCR_r(E);
		TS_ASSERT_EQUALS(cpu.reg[E], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		cpu.DCR_r(A);
		TS_ASSERT_EQUALS(cpu.reg[A], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 35);
	}
	void test_DCR_m() {
		cpu.reg[H] = 0xba;
		cpu.reg[L] = 0xbe;
		cpu.INR_m();
		cpu.INR_m();
		cpu.cycles = 0;
		cpu.DCR_m();
		TS_ASSERT_EQUALS(cpu.memory[cpu.getHL()], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.DCR_m();
		TS_ASSERT_EQUALS(cpu.memory[cpu.getHL()], 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);

		cpu.reg[H] = 0xde;
		cpu.reg[L] = 0xad;
		cpu.INR_m();
		cpu.INR_m();
		cpu.cycles = 20;
		cpu.DCR_m();
		TS_ASSERT_EQUALS(cpu.memory[cpu.getHL()], 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		cpu.DCR_m();
		TS_ASSERT_EQUALS(cpu.memory[cpu.getHL()], 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);
	}
	void test_INX_r() {
		// Test basic increment
		cpu.INX_r(B);
		TS_ASSERT_EQUALS(cpu.reg[B], 0x0);
		TS_ASSERT_EQUALS(cpu.reg[C], 0x1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);
		cpu.INX_r(D);
		TS_ASSERT_EQUALS(cpu.reg[D], 0x0);
		TS_ASSERT_EQUALS(cpu.reg[E], 0x1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.INX_r(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 0x0);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 15);

		// Test rollover
		cpu.reg[C] = 0xff;
		cpu.reg[D] = 0x32;
		cpu.reg[E] = 0xff;
		cpu.reg[H] = 0xba;
		cpu.reg[L] = 0xff;

		cpu.INX_r(B);
		TS_ASSERT_EQUALS(cpu.reg[B], 0x01);
		TS_ASSERT_EQUALS(cpu.reg[C], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.INX_r(D);
		TS_ASSERT_EQUALS(cpu.reg[D], 0x33);
		TS_ASSERT_EQUALS(cpu.reg[E], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 25);
		cpu.INX_r(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 0xbb);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x00);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);
	}
	void test_DCX_r() {
		cpu.reg[C] = 0x1;
		cpu.reg[E] = 0x1;
		cpu.reg[L] = 0x1;

		// Test basic increment
		cpu.DCX_r(B);
		TS_ASSERT_EQUALS(cpu.reg[B], 0x0);
		TS_ASSERT_EQUALS(cpu.reg[C], 0x0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 5);
		cpu.DCX_r(D);
		TS_ASSERT_EQUALS(cpu.reg[D], 0x0);
		TS_ASSERT_EQUALS(cpu.reg[E], 0x0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
		cpu.DCX_r(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 0x0);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 15);

		// Test rollover
		cpu.reg[B] = 0xff;
		cpu.reg[C] = 0x00;
		cpu.reg[D] = 0x33;
		cpu.reg[E] = 0x00;
		cpu.reg[H] = 0xbb;
		cpu.reg[L] = 0x00;

		cpu.DCX_r(B);
		TS_ASSERT_EQUALS(cpu.reg[B], 0xfe);
		TS_ASSERT_EQUALS(cpu.reg[C], 0xff);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);
		cpu.DCX_r(D);
		TS_ASSERT_EQUALS(cpu.reg[D], 0x32);
		TS_ASSERT_EQUALS(cpu.reg[E], 0xff);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 25);
		cpu.DCX_r(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 0xba);
		TS_ASSERT_EQUALS(cpu.reg[L], 0xff);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);
	}
};

class AddTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;

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
		cpu.resetFlags();
		cyclesTmp = cpu.cycles;
	}

	void test_ADD() {
		cpu.ADD_r(B);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x15);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.ADD_r(C);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x21);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.ADD_r(D);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x2e);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);

		cpu.ADD_r(E);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x3c);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.ADD_r(H);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x4e);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);

		cpu.ADD_r(A);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x9c);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 24);

		cpu.reg[H] = 0x64;
		cpu.ADD_r(H);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);

		cpu.reg[B] = 0x2;
		cpu.ADD_r(B);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x2);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 32);

		cpu.memory[cpu.getHL()] = 0xfd;
		cpu.ADD_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 39);

		cpu.memory[cpu.getHL()] = 0x1;
		cpu.ADD_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 46);
	}

	void test_ADC() {
		cpu.ADC_r(B);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x15);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.reg[C] = 0xec;
		cpu.ADC_r(C);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.ADC_r(D);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xf);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);

		cpu.memory[cpu.getHL()] = 0xf0;
		cpu.ADC_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 19);

		cpu.memory[cpu.getHL()] = 0x1;
		cpu.ADC_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 26);

		cpu.memory[cpu.getHL()] = 0x1;
		cpu.ADC_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x2);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 33);
	}

	void test_ADI() {
		cpu.ADI(0xba);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xc4);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 7);

		cpu.ADI(0xbe);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x82);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 14);

		cpu.ADI(0x7e);
		TS_ASSERT_EQUALS(cpu.reg[A], 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 21);

		cpu.ADI(0x0);
		TS_ASSERT_EQUALS(cpu.reg[A], 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
	}

	void test_ACI() {
		cpu.ACI(0xba);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xc4);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 7);

		cpu.ACI(0xbe);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x82);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 14);

		cpu.ACI(0x7d);
		TS_ASSERT_EQUALS(cpu.reg[A], 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 21);

		cpu.ACI(0x0);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
	}

	void test_DAD() {
		cpu.DAD_r(B);
		TS_ASSERT_EQUALS(cpu.reg[H], 0x1d);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x23);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);

		cpu.DAD_r(D);
		TS_ASSERT_EQUALS(cpu.reg[H], 0x2a);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x31);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 20);

		cpu.DAD_r(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 0x54);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x62);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);

		cpu.DAD_r(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 0xa8);
		TS_ASSERT_EQUALS(cpu.reg[L], 0xc4);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 40);

		cpu.DAD_r(H);
		TS_ASSERT_EQUALS(cpu.reg[H], 0x51);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x88);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 50);

		cpu.sp = 1;
		cpu.DAD_SP();
		TS_ASSERT_EQUALS(cpu.reg[H], 0x51);
		TS_ASSERT_EQUALS(cpu.reg[L], 0x89);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 60);
	}
};

class SubtractTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;

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
		cpu.resetFlags();
		cyclesTmp = cpu.cycles;
	}

	void test_SUB() {
		cpu.SUB_r(B);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.SUB_r(D);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xf2)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.SUB_r(H);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xe0)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);

		cpu.memory[cpu.getHL()] = 0xd1;
		cpu.SUB_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xf)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 19);
	}

	void test_SBB() {
		cpu.SBB_r(B);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.SBB_r(H);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xec)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.memory[cpu.getHL()] = 0xdd;
		cpu.SBB_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xf)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 15);
	}

	void test_SUI() {
		cpu.SUI(0xba);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x50)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 7);

		cpu.SUI(0xaa);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xa6)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 14);

		cpu.SUI(0x7);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x9f)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 21);

		cpu.SUI(0x1);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x9e)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
	}
	void test_SBI() {
		cpu.SBI(0xba);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x50)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 7);

		cpu.SBI(0xaa);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xa5)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 14);

		cpu.SBI(0x7);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x9d)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 21);

		cpu.SBI(0x1);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x9c)
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 28);
	}
};

class LogicalTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;

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
		cpu.resetFlags();
		cyclesTmp = cpu.cycles;
	}
	void test_ANA() {
		cpu.ANA_r(B);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xa);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.ANA_r(C);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x8);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.reg[D] = 0x8;
		cpu.ANA_r(D);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x8);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);

		cpu.ANA_r(A);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x8);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.ANI(0xff);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x8);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 23);

		cpu.memory[cpu.getHL()] = 0x00;
		cpu.ANA_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);
	}
	void test_XRA() {
		cpu.XRA_r(B);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.XRA_r(C);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xd);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.reg[D] = 0x69;
		cpu.XRA_r(D);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x64);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);

		cpu.XRA_r(A);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.XRI(0xff);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 23);

		cpu.memory[cpu.getHL()] = 0x00;
		cpu.XRA_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);
	}
	void test_ORA() {
		cpu.ORA_r(B);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xb);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.ORA_r(C);
		TS_ASSERT_EQUALS(cpu.reg[A], 0xf);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.reg[D] = 0x69;
		cpu.ORA_r(D);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x6f);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);

		cpu.ORA_r(A);
		TS_ASSERT_EQUALS(cpu.reg[A], 0x6f);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);

		cpu.memory[cpu.getHL()] = 0xff;
		cpu.ORA_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 23);

		cpu.memory[cpu.getHL()] = 0x00;
		cpu.ORA_m();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 30);
	}
	void test_CMP() {
		cpu.reg[A] = 0x80;

		cpu.CMP_r(B);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.reg[C] = 0x90;
		cpu.CMP_r(C);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.memory[cpu.getHL()] = 0x80;
		cpu.CMP_m();
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 15);

		cpu.reg[A] = 0x60;
		cpu.CPI(0xf0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 22);
	}
};

class RotateTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;

	void setUp(void) {
		// cpu = Intel8080();
		for (int i = 0; i <= 0xffff; i++) cpu.memory[i] = 0;
		cpu.reg[B] = 0xb;
		cpu.reg[C] = 0xc;
		cpu.reg[D] = 0xd;
		cpu.reg[E] = 0xe;
		cpu.reg[H] = 0x12;
		cpu.reg[L] = 0x17;
		cpu.reg[A] = 0Xff;
		cyclesTmp = cpu.cycles;
	}
	void test_RLC() {
		cpu.RLC();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.reg[A] = 0x80;
		cpu.RLC();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.reg[A] = 0x66;
		cpu.RLC();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xcc);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);
	}
	void test_RRC() {
		cpu.RRC();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xff);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.reg[A] = 0x1;
		cpu.RRC();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x80);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.reg[A] = 0xcc;
		cpu.RRC();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x66);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);
	}
	void test_RAL() {
		cpu.RAL();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xfe);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.reg[A] = 0x80;
		cpu.RAL();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.reg[A] = 0x66;
		cpu.RAL();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xcc);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);
	}
	void test_RAR() {
		cpu.RAR();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x7f);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.reg[A] = 0x1;
		cpu.RAR();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x00);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.reg[A] = 0xcc;
		cpu.RAR();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x66);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);
	}
};

class SpecialsTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;

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
		cpu.resetFlags();
		cyclesTmp = cpu.cycles;
	}
	void test_CMA() {
		cpu.CMA();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xf5);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.CMA();
		TS_ASSERT_EQUALS(cpu.reg[A], 0xa);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);
	}
	void test_STC() {
		cpu.STC();
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);
	}
	void test_CMC() {
		cpu.CMC();
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);
		cpu.CMC();
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);
	}
	void test_DAA() {
		cpu.resetFlags();
		cpu.reg[A] = 0xb;
		cpu.DAA();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x11);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);

		cpu.resetFlags();
		cpu.reg[A] = 0xef;
		cpu.DAA();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x55);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 8);

		cpu.resetFlags();
		cpu.reg[A] = 0x8b;
		cpu.DAA();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x91);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 1);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 0);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 12);

		cpu.setFlag(STATUS_AUX_CARRY, 1);
		cpu.reg[A] = 0x10;
		cpu.DAA();
		TS_ASSERT_EQUALS(cpu.reg[A], 0x16);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_AUX_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_CARRY), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_ZERO), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_SIGN), 0);
		TS_ASSERT_EQUALS(cpu.getFlag(STATUS_PARITY), 1);
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 16);
	}
};

class InputOuputTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;

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
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
	}
	void test_OUT() {
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 10);
	}
};

class ControlTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;

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
		cpu.inte = false;
		cpu.EI();
		TS_ASSERT(cpu.inte);
	}
	void test_DI() {
		cpu.inte = true;
		cpu.DI();
		TS_ASSERT(!cpu.inte);
	}
	void test_NOP() {
		cpu.NOP();
		TS_ASSERT_EQUALS(cpu.cycles, cyclesTmp + 4);
	}
	void test_HLT() {
		uint16_t tmpPC = cpu.pc;
		cpu.HLT();
		TS_ASSERT(cpu.halt);
	}
};

class ResetTests : public CxxTest::TestSuite {
public:
	Intel8080 cpu;
	uint32_t cyclesTmp;

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
	uint32_t cyclesTmp;

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
};
