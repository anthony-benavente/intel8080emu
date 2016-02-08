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
	
	void test_RST() {
	    TS_ASSERT(false);
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

	/* -------------------------------------------- */
	/* STACK Instructions
	/* -------------------------------------------- */
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
	/* -------------------------------------------- */
	/* /STACK Instructions
	/* -------------------------------------------- */


	/* -------------------------------------------- */
	/* JUMP Instructions
	/* -------------------------------------------- */
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
	/* -------------------------------------------- */
	/* /JUMP Instructions
	/* -------------------------------------------- */


	/* -------------------------------------------- */
	/* CALL Instructions
	/* -------------------------------------------- */

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
	/* -------------------------------------------- */
	/* /CALL Instructions
	/* -------------------------------------------- */

	/* -------------------------------------------- */
	/* RETURN Instructions
	/* -------------------------------------------- */

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
	/* -------------------------------------------- */
	/* /RETURN Instructions
	/* -------------------------------------------- */


	/* -------------------------------------------- */
	/* INCREMENT DECREMENT Instructions
	/* -------------------------------------------- */
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
	/* -------------------------------------------- */
	/* /INCREMENT DECREMENT Instructions
	/* -------------------------------------------- */

	/* -------------------------------------------- */
	/* /INCREMENT DECREMENT Instructions
	/* -------------------------------------------- */

	/* -------------------------------------------- */
	/* /ADD Instructions
	/* -------------------------------------------- */
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
	/* -------------------------------------------- */
	/* /ADD Instructions
	/* -------------------------------------------- */


	/* -------------------------------------------- */
	/* SUBTRACT Instructions
	/* -------------------------------------------- */
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
	/* -------------------------------------------- */
	/* /SUBTRACT Instructions
	/* -------------------------------------------- */

	/* -------------------------------------------- */
	/* LOGICAL Instructions
	/* -------------------------------------------- */
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
	/* -------------------------------------------- */
	/* /LOGICAL Instructions
	/* -------------------------------------------- */


	/* -------------------------------------------- */
	/* ROTATE Instructions
	/* -------------------------------------------- */
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
	/* -------------------------------------------- */
	/* /ROTATE Instructions
	/* -------------------------------------------- */

	/* -------------------------------------------- */
	/* SPECIALS Instructions
	/* -------------------------------------------- */
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
	/* -------------------------------------------- */
	/* /SPECIALS Instructions
	/* -------------------------------------------- */

	/* -------------------------------------------- */
	/* INPUT/OUTPUT Instructions
	/* -------------------------------------------- */
	void test_IN() {
	    TS_ASSERT(false);
	}
	void test_OUT() {
	    TS_ASSERT(false);
	}
	/* -------------------------------------------- */
	/* /INPUT/OUTPUT Instructions
	/* -------------------------------------------- */

	/* -------------------------------------------- */
	/* CONTROL Instructions
	/* -------------------------------------------- */

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
	/* -------------------------------------------- */
	/* /CONTROL Instructions
	/* -------------------------------------------- */

};
