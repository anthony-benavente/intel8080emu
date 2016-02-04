#include "cxxtest/TestSuite.h"

#include "cpu/intel8080.hpp"

class Intel8080TestSuite : public CxxTest::TestSuite {
	static Intel8080 cpu;
	static program_t *program;

public:
    static Intel8080TestSuite* createSuite() {
		printf("Got here");
		program = getProgram("/home/anthony/programming/cpp/intel8080/res/invaders.rom");
		cpu.loadProgram(program);
		return new Intel8080TestSuite();
    }

	static void destroySuite(Intel8080TestSuite *suite) {
		delete suite;
		free_program(program);
	}

	void destroySuite() {
		free_program(program);
	}

	void test_getNextOp(void) {
		for (int i = 0; i < program->size; i++) {
			printf("(0x%x) ", program->data[i]);
			if (i % 20 == 0) printf("\n");
		}
		TS_ASSERT(true);
	}
	void test_decode() {
	    TS_ASSERT(false);
	}
	void test_NOP() {
	    TS_ASSERT(false);
	}
	void test_SHLD_A16() {
	    TS_ASSERT(false);
	}
	void test_STA_A16() {
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
	void test_XCHG() {
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
	void test_LXI_B() {
	    TS_ASSERT(false);
	}
	void test_LXI_D() {
	    TS_ASSERT(false);
	}
	void test_STAX_B() {
	    TS_ASSERT(false);
	}
	void test_STAX_D() {
	    TS_ASSERT(false);
	}
	void test_LXI_H() {
	    TS_ASSERT(false);
	}
	void test_LXI_SP() {
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
	void test_MVI_B() {
	    TS_ASSERT(false);
	}
	void test_MVI_D() {
	    TS_ASSERT(false);
	}
	void test_MVI_H() {
	    TS_ASSERT(false);
	}
	void test_MVI_M() {
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
	void test_LDAX_B() {
	    TS_ASSERT(false);
	}
	void test_LDAX_D() {
	    TS_ASSERT(false);
	}
	void test_LHLD() {
	    TS_ASSERT(false);
	}
	void test_LDA() {
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
	void test_MVI_C() {
	    TS_ASSERT(false);
	}
	void test_MVI_E() {
	    TS_ASSERT(false);
	}
	void test_MVI_L() {
	    TS_ASSERT(false);
	}
	void test_MVI_A() {
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
	void test_MOV() {
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
