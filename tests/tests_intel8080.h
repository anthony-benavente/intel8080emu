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
		// Test pushing B,C to stack
		// Test pushing H,L to stack
		// Test pushing PSW to stack (A, status)
	    TS_ASSERT(false);
	}
	void test_POP() {
		// Test popping from stack into B pair
		// Test popping from stack into H pair
		// Test popping from stack into PSW pair
		TS_ASSERT(false);
	}
	void test_XTHL() {
		// Test exchanging stack[sp, sp+1] with L,H respectively
	    TS_ASSERT(false);
	}
	void test_SPHL() {
		// Test value H << 8 | L getting loaded into stack pointer
	    TS_ASSERT(false);
	}
	void test_LXI_SP() {
		// Test sp becomes 3(data) << 8 | 2(data)
		TS_ASSERT(false);
	}
	void test_INX_SP() {
		// Test increase value of SP by 1
	    TS_ASSERT(false);
	}
	void test_DCX_SP() {
		// Test decrease value of SP by 1
	    TS_ASSERT(false);
	}
};

class JumpTests : public CxxTest::TestSuite {
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
	void test_JMP() {
		// Test setting the program counter to 3(data) << 8 | 2(data)
		TS_ASSERT(false);
	}
	void test_JC() {
		// Test JMP called if carry bit one
	    TS_ASSERT(false);
	}
	void test_JNC() {
		// Test JMP called if carry bit 0
	    TS_ASSERT(false);
	}
	void test_JZ() {
		// Test JMP called if zero bit one
	    TS_ASSERT(false);
	}
	void test_JNZ() {
		// Test JMP called if zero bit 0
	    TS_ASSERT(false);
	}
	void test_JP() {
		// Test JMP called if sign bit 0
	    TS_ASSERT(false);
	}
	void test_JM() {
		// Test JMP called if sign bit 1
	    TS_ASSERT(false);
	}
	void test_JPE() {
		// Test JMP called if parity 1
	    TS_ASSERT(false);
	}
	void test_JPO() {
		// Test JMP called if parity 0
	    TS_ASSERT(false);
	}
	void test_PCHL() {
		// Test loading H << 8 | L into program counter
	    TS_ASSERT(false);
	}
};

class CallTests : public CxxTest::TestSuite {
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

	void test_CALL() {
	    TS_ASSERT(false);
	}
	void test_CC() {
	    TS_ASSERT(false);
	}
	void test_CNC() {
	    TS_ASSERT(false);
	}
	void test_CZ() {
	    TS_ASSERT(false);
	}
	void test_CNZ() {
	    TS_ASSERT(false);
	}
	void test_CP() {
	    TS_ASSERT(false);
	}
	void test_CM() {
	    TS_ASSERT(false);
	}
	void test_CPE() {
	    TS_ASSERT(false);
	}
	void test_CPO() {
	    TS_ASSERT(false);
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
			TS_ASSERT_EQUALS(cpu.stack.top(), i - 1);
			cpu.stack.pop();
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

	void test_decode() {
	    TS_ASSERT(false);
	}
};
