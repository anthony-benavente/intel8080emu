#include "cxxtest/TestSuite.h"

#include "program/program.hpp"
#include "cpu/intel8080.hpp"

class Intel8080TestSuite : public CxxTest::TestSuite {
	Intel8080 cpu;
public:

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

	/* -------------------------------------------- */
	/*   MOVE LOAD STORE Instructions				*/
	/* -------------------------------------------- */
	void test_MOV() {
		// Test moving register to register
		// Test moving register to memory
		// Test moving register to memory
		TS_ASSERT(false);
	}

	void test_MVI() {
		// Test moving immediate to memory at H,L pair
		// Test moving immediate to register
		TS_ASSERT(false);
	}

	void test_LXI() {
		// Test load immediate to pair B,C
		// Test load immediate to pair D,E
		// Test load immediate to pair H,L
	    TS_ASSERT(false);
	}

	void test_STAX() {
		// Test store accumulator into memory @ B,C
		// Test store accumulator into memory @ D,E
	    TS_ASSERT(false);
	}

	void test_LDAX() {
		// Test load accumulator from memory @ B,C
		// Test load accumulator from memory @ D,E
		TS_ASSERT(false);
	}

	void test_STA() {
		cpu.reg[7] = 0xae;
		cpu.pc = 0x9990;
		cpu.memory[0x9990] = 0xa0;
		cpu.memory[0x9991] = 0xaa;
		cpu.cycles = 0;

		TS_ASSERT_EQUALS(cpu.memory[0xaaa0], 0);
		TS_ASSERT_EQUALS(cpu.memory[0xaaa1], 0);
		cpu.STA_A16();
		TS_ASSERT_EQUALS(cpu.memory[0xaaa0], 0x00);
		TS_ASSERT_EQUALS(cpu.memory[0xaaa1], 0xae);
		TS_ASSERT_EQUALS(cpu.cycles, 13);

		cpu.pc = 0;
		cpu.reg[7] = 0;
		cpu.memory[0x9990] = 0;
		cpu.memory[0x9991] = 0;
		cpu.memory[0xaaa0] = 0;
		cpu.memory[0xaaa1] = 0;
		cpu.cycles = 0;
	}

	void test_LDA() {
		// Test loading memory @ [pc + 1:pc + 2] into acculumator
	    TS_ASSERT(false);
	}

	void test_SHLD() {
		cpu.reg[4] = 0xae;
		cpu.reg[5] = 0x29;
		cpu.pc = 0x9990;
		cpu.memory[0x9990] = 0xa0;
		cpu.memory[0x9991] = 0xaa;
		cpu.cycles = 0;

		TS_ASSERT_EQUALS(cpu.memory[0xaaa0], 0);
		TS_ASSERT_EQUALS(cpu.memory[0xaaa1], 0);
		cpu.SHLD_A16();
		TS_ASSERT_EQUALS(cpu.memory[0xaaa0], 0x29);
		TS_ASSERT_EQUALS(cpu.memory[0xaaa1], 0xae);
		TS_ASSERT_EQUALS(cpu.cycles, 16);

		cpu.pc = 0;
		cpu.reg[4] = 0;
		cpu.reg[5] = 0;
		cpu.memory[0x9990] = 0;
		cpu.memory[0x9991] = 0;
		cpu.cycles = 0;
	}

	void test_LHLD() {
		// Load memory @ [pc + 1:pc + 2] -> L, H respectively
	    TS_ASSERT(false);
	}

	void test_XCHG() {
		// Swap pair D,E with H,L respectively
	    TS_ASSERT(false);
	}

	/* -------------------------------------------- */
	/* /MOVE LOAD STORE Instructions
	/* -------------------------------------------- */

	void test_LXI_SP() {
		TS_ASSERT(false);
	}
	void test_RLC() {
	    TS_ASSERT(false);
	}
	void test_RAL() {
	    TS_ASSERT(false);
	}
	void test_DAA() {
	    TS_ASSERT(false);
	}
	void test_STC() {
	    TS_ASSERT(false);
	}
	void test_RRC() {
	    TS_ASSERT(false);
	}
	void test_RAR() {
	    TS_ASSERT(false);
	}
	void test_CMA() {
	    TS_ASSERT(false);
	}
	void test_CMC() {
	    TS_ASSERT(false);
	}
	void test_HLT() {
	    TS_ASSERT(false);
	}
	void test_RNZ() {
	    TS_ASSERT(false);
	}
	void test_RNC() {
	    TS_ASSERT(false);
	}
	void test_RPO() {
	    TS_ASSERT(false);
	}
	void test_RP() {
	    TS_ASSERT(false);
	}
	void test_JNZ() {
	    TS_ASSERT(false);
	}
	void test_JNC() {
	    TS_ASSERT(false);
	}
	void test_JPO() {
	    TS_ASSERT(false);
	}
	void test_JP() {
	    TS_ASSERT(false);
	}
	void test_JMP() {
	    TS_ASSERT(false);
	}
	void test_OUT() {
	    TS_ASSERT(false);
	}
	void test_XTHL() {
	    TS_ASSERT(false);
	}
	void test_DI() {
	    TS_ASSERT(false);
	}
	void test_CNZ() {
	    TS_ASSERT(false);
	}
	void test_CNC() {
	    TS_ASSERT(false);
	}
	void test_CPO() {
	    TS_ASSERT(false);
	}
	void test_CP() {
	    TS_ASSERT(false);
	}
	void test_ADI() {
	    TS_ASSERT(false);
	}
	void test_USI() {
	    TS_ASSERT(false);
	}
	void test_ANI() {
	    TS_ASSERT(false);
	}
	void test_ORI() {
	    TS_ASSERT(false);
	}
	void test_RZ() {
	    TS_ASSERT(false);
	}
	void test_RC() {
	    TS_ASSERT(false);
	}
	void test_RPE() {
	    TS_ASSERT(false);
	}
	void test_RM() {
	    TS_ASSERT(false);
	}
	void test_RET() {
	    TS_ASSERT(false);
	}
	void test_PCHL() {
	    TS_ASSERT(false);
	}
	void test_SPHL() {
	    TS_ASSERT(false);
	}
	void test_JZ() {
	    TS_ASSERT(false);
	}
	void test_JC() {
	    TS_ASSERT(false);
	}
	void test_JPE() {
	    TS_ASSERT(false);
	}
	void test_JM() {
	    TS_ASSERT(false);
	}
	void test_IN() {
	    TS_ASSERT(false);
	}
	void test_EI() {
	    TS_ASSERT(false);
	}
	void test_CZ() {
	    TS_ASSERT(false);
	}
	void test_CC() {
	    TS_ASSERT(false);
	}
	void test_CPE() {
	    TS_ASSERT(false);
	}
	void test_CM() {
	    TS_ASSERT(false);
	}
	void test_CALL() {
	    TS_ASSERT(false);
	}
	void test_ACI() {
	    TS_ASSERT(false);
	}
	void test_SBI() {
	    TS_ASSERT(false);
	}
	void test_INX_B() {
	    TS_ASSERT(false);
	}
	void test_INX_D() {
	    TS_ASSERT(false);
	}
	void test_INX_H() {
	    TS_ASSERT(false);
	}
	void test_INX_SP() {
	    TS_ASSERT(false);
	}
	void test_INR_B() {
	    TS_ASSERT(false);
	}
	void test_INR_D() {
	    TS_ASSERT(false);
	}
	void test_INR_H() {
	    TS_ASSERT(false);
	}
	void test_INR_M() {
	    TS_ASSERT(false);
	}
	void test_DCR_B() {
	    TS_ASSERT(false);
	}
	void test_DCR_D() {
	    TS_ASSERT(false);
	}
	void test_DCR_H() {
	    TS_ASSERT(false);
	}
	void test_DCR_M() {
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
	void test_DCX_B() {
	    TS_ASSERT(false);
	}
	void test_DCX_D() {
	    TS_ASSERT(false);
	}
	void test_DCX_H() {
	    TS_ASSERT(false);
	}
	void test_DCX_SP() {
	    TS_ASSERT(false);
	}
	void test_INR_C() {
	    TS_ASSERT(false);
	}
	void test_INR_E() {
	    TS_ASSERT(false);
	}
	void test_INR_L() {
	    TS_ASSERT(false);
	}
	void test_INR_A() {
	    TS_ASSERT(false);
	}
	void test_DCR_C() {
	    TS_ASSERT(false);
	}
	void test_DCR_E() {
	    TS_ASSERT(false);
	}
	void test_DCR_L() {
	    TS_ASSERT(false);
	}
	void test_DCR_A() {
	    TS_ASSERT(false);
	}
	void test_POP_B() {
	    TS_ASSERT(false);
	}
	void test_POP_D() {
	    TS_ASSERT(false);
	}
	void test_POP_H() {
	    TS_ASSERT(false);
	}
	void test_POP_PSW() {
	    TS_ASSERT(false);
	}
	void test_PUSH_B() {
	    TS_ASSERT(false);
	}
	void test_PUSH_D() {
	    TS_ASSERT(false);
	}
	void test_PUSH_H() {
	    TS_ASSERT(false);
	}
	void test_PUSH_PSW() {
	    TS_ASSERT(false);
	}
	void test_RST0() {
	    TS_ASSERT(false);
	}
	void test_RST2() {
	    TS_ASSERT(false);
	}
	void test_RST4() {
	    TS_ASSERT(false);
	}
	void test_RST6() {
	    TS_ASSERT(false);
	}
	void test_RST1() {
	    TS_ASSERT(false);
	}
	void test_RST3() {
	    TS_ASSERT(false);
	}
	void test_RST5() {
	    TS_ASSERT(false);
	}
	void test_RST7() {
	    TS_ASSERT(false);
	}
	void test_ADD() {
	    TS_ASSERT(false);
	}
	void test_ADC() {
	    TS_ASSERT(false);
	}
	void test_SUB() {
	    TS_ASSERT(false);
	}
	void test_SBB() {
	    TS_ASSERT(false);
	}
	void test_ANA() {
	    TS_ASSERT(false);
	}
	void test_XRA() {
	    TS_ASSERT(false);
	}
	void test_ORA() {
	    TS_ASSERT(false);
	}
	void test_CMP() {
	    TS_ASSERT(false);
	}

};
