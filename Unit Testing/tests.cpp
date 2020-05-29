#define CATCH_CONFIG_MAIN
#include "/Users/jmm_1/Desktop/invadersrom/catch.hpp"
#include "../8080emulator/cpu.h"
#include "../8080emulator/instructions.h"
#include "../8080emulator/inout.h"

uint8_t RAM[0xffff]{}; //to test memory access

//verify pair formation works as expected
TEST_CASE("Register pair formation") {
	state->B = 0x12;
	state->C = 0x34;
	state->BC_pair = state->pair(state->B, state->C);
	REQUIRE(state->BC_pair == 0x1234);
}

//test the shift register
TEST_CASE("Shift register") {
	SECTION("OUT port 4") {
		mOUT(4, 0xab);
		REQUIRE(shift16.status == 0xab00);
		mOUT(4, 0xfe);
		REQUIRE(shift16.status == 0xfeab);
	}
	SECTION("OUT port 2 and IN port 3") {
		shift16.status = 0x3080;
		mOUT(2, 3);
		mIN(3);
		REQUIRE(state->A == 0x84);
	}
}

//Data transfer group
TEST_CASE("MOV and MVI instructions") {
	state->A = 0xaa;
	MOV(state->B, state->A);
	MOV(state->C, state->B);
	REQUIRE(state->B == state->A);

	SECTION("move from/to memory") {
		RAM[2] = 0xb2;
		MOV(RAM[2], state->A);
		MOV(state->C, state->B);
		REQUIRE(state->B == RAM[2]);
		state->B = 0x11;
		MOV(state->A, state->B);
		MOV(RAM[2], state->A);
		REQUIRE(state->B == RAM[2]);
	}
}

TEST_CASE("LXI instruction") {
	LXI(state->H, state->L, 0x12, 0x34);
	REQUIRE(state->H == 0x34);
	REQUIRE(state->L == 0x12);
	state->HL_pair = state->pair(state->H, state->L);
	REQUIRE(state->HL_pair == 0x3412);

	SECTION("Special case: LXI SP") {
		LXI_SP( 0xaa, 0xff);
		REQUIRE(state->SP == 0xffaa);
	}
}

TEST_CASE("Test LDA and STA") {
	RAM[0x1012] = 0x45;
	state->memory = RAM;
	uint8_t byte2 = 0x12;
	uint8_t byte3 = 0x10;
	SECTION("LDA test") {
		LDA(byte2, byte3);
		REQUIRE(state->A == 0x45);
	}
	SECTION("STA test") {
		state->A = 0xe9;
		STA(byte2, byte3);
		REQUIRE(RAM[0x1012] == state->A);
	}
}

TEST_CASE("LHLD and SHLD test") {
	RAM[0x1006] = 0x34;
	RAM[0x1007] = 0x12;
	state->memory = RAM;
	uint8_t byte2 = 0x06;
	uint8_t byte3 = 0x10;
	SECTION("LHLD") {
		LHLD(byte2, byte3);
		REQUIRE(state->HL_pair == 0x1234);
	}
	SECTION("SHLD") {
		state->H = 0xf0;
		state->L = 0xfa;
		SHLD(byte2, byte3);
		REQUIRE(RAM[0x1006] == state->L);
		REQUIRE(RAM[0x1007] == state->H);
	}
}

TEST_CASE("LDAX and STAX") {
	RAM[0x1006] = 0x34;
	state->memory = RAM;
	SECTION("LDAX") {
		state->BC_pair = 0x1006;
		LDAX(state->BC_pair);
		REQUIRE(state->A == 0x34);
	}
	SECTION("STAX") {
		state->BC_pair = 0x1006;
		state->A = 0xcf;
		STAX(state->BC_pair);
		REQUIRE(RAM[0x1006] == 0xcf);
	}
}

TEST_CASE("XCHG") {
	state->D = 0x0d;
	state->E = 0x0e;
	state->H = 0x0f;
	state->L = 0x01;
	XCHG();
	REQUIRE(state->D == 0x0f);
	REQUIRE(state->L == 0x0e);
	REQUIRE(state->HL_pair == 0x0d0e);
	REQUIRE(state->DE_pair == 0x0f01);
}

//Arithmetic group
TEST_CASE("ADD, ADD M and ADI") {
	SECTION("ADD") {
		state->B = 0x01;
		SECTION("simple sum") {
			state->A = 0x02;
			ADD(state->B);
			REQUIRE(state->A == 0x03);
		}
		SECTION("zero, parity and carry flags") {
			state->A = 0xff;
			ADD(state->B);
			REQUIRE(state->A == 0x00);
			REQUIRE(state->flag.Z == true);
			REQUIRE(state->flag.C == true);
			REQUIRE(state->flag.S == false);
			REQUIRE(state->flag.P == true);
		}
	}

	SECTION("ADD M") {
		RAM[0x3456] = 0x02;
		state->memory = RAM;
		uint8_t byte = state->memory[0x3456];
		SECTION("simple sum") {
			state->A = 0x01;
			ADD(byte);
			REQUIRE(state->A == 0x03);
		}
		SECTION("zero, parity and carry flags") {
			state->A = 0xff;
			ADD(state->B);
			REQUIRE(state->A == 0x00);
			REQUIRE(state->flag.Z == true);
			REQUIRE(state->flag.C == true);
			REQUIRE(state->flag.S == false);
			REQUIRE(state->flag.P == true);
		}
	}

	SECTION("ADI") {
		uint8_t byte = 0x01;
		SECTION("simple sum") {
			state->A = 0x02;
			ADD(byte);
			REQUIRE(state->A == 0x03);
		}
		SECTION("zero, parity and carry flags") {
			state->A = 0xff;
			ADD(state->B);
			REQUIRE(state->A == 0x00);
			REQUIRE(state->flag.Z == true);
			REQUIRE(state->flag.C == true);
			REQUIRE(state->flag.S == false);
			REQUIRE(state->flag.P == true);
		}
	}
}

TEST_CASE("ADC, ADC M and ACI") {
	state->flag.C = true;
	SECTION("ADC") {
		state->B = 0x01;
		SECTION("simple sum") {
			state->A = 0x02;
			ADC(state->B);
			REQUIRE(state->A == 0x04);
		}
		SECTION("zero, parity and carry flags") {
			state->A = 0xff;
			ADC(state->B);
			REQUIRE(state->A == 0x01);
			REQUIRE(state->flag.Z == false);
			REQUIRE(state->flag.C == true);
			REQUIRE(state->flag.S == false);
			REQUIRE(state->flag.P == false);
		}
	}

	SECTION("ACI M") {
		RAM[0x3456] = 0x02;
		state->memory = RAM;
		uint8_t byte = state->memory[0x3456];
		SECTION("simple sum") {
			state->A = 0x01;
			ADC(byte);
			REQUIRE(state->A == 0x04);
		}
		SECTION("zero, parity and carry flags") {
			state->A = 0xff;
			ADC(state->B);
			REQUIRE(state->A == 0x01);
			REQUIRE(state->flag.Z == false);
			REQUIRE(state->flag.C == true);
			REQUIRE(state->flag.S == false);
			REQUIRE(state->flag.P == false);
		}
	}

	SECTION("ACI") {
		uint8_t byte = 0x01;
		SECTION("simple sum") {
			state->A = 0x01;
			ADC(byte);
			REQUIRE(state->A == 0x03);
		}
		SECTION("zero, parity and carry flags") {
			state->A = 0xff;
			ADC(state->B);
			REQUIRE(state->A == 0x01);
			REQUIRE(state->flag.Z == false);
			REQUIRE(state->flag.C == true);
			REQUIRE(state->flag.S == false);
			REQUIRE(state->flag.P == false);
		}
	}
}

TEST_CASE("SUB, SUB M and SUI") {
	state->A = 0x05;
	SECTION("SUB") {
		state->D = 0x05;
		SUB(state->D);
		REQUIRE(state->A == 0);
		REQUIRE(state->flag.Z == true);
	}
	SECTION("SUB M") {
		state->A = 0x00;
		RAM[0x3456] = 0x07;
		state->memory = RAM;
		uint8_t byte = state->memory[0x3456];
		SUB(byte);
		REQUIRE(state->A == 0xf9);
	}
}

TEST_CASE("SBB, SBB M and SBI") {
	state->A = 0x05;
	state->flag.C = true;
	SECTION("SBB") {
		state->D = 0x05;
		SBB(state->D);
		REQUIRE(state->A == 0xff);
		REQUIRE(state->flag.C == true);
	}
	SECTION("SUB M") {
		state->A = 0x00;
		RAM[0x3456] = 0x07;
		state->memory = RAM;
		uint8_t byte = state->memory[0x3456];
		SBB(byte);
		REQUIRE(state->A == 0xf8);
		REQUIRE(state->flag.S == true);
	}
}

TEST_CASE("INR, INR M, DCR, DCR M") {
	RAM[0xfafa] = 0x01;
	state->memory = RAM;
	state->E = state->memory[0xfafa];
	INR(state->E);
	DCR(state->E);
	DCR(state->E);
	REQUIRE(state->E == 0);
	REQUIRE(state->flag.Z == true);
	REQUIRE(state->flag.P == true);
}

TEST_CASE("INX and DCX") {
	state->B = 0x11;
	state->C = 0xff;
	INX(state->B, state->C);
	INX(state->B, state->C);
	INX(state->B, state->C);
	DCX(state->B, state->C);
	REQUIRE(state->B == 0x12);
	REQUIRE(state->C == 0x01);
	REQUIRE(state->BC_pair == 0x1201);
}

TEST_CASE("DAD") {
	state->flag.C = false;
	state->HL_pair = 0xfafa;
	state->D = 0xaa;
	state->E = 0xff;
	DAD(state->D, state->E);
	REQUIRE(state->HL_pair == 0xa5f9);
	REQUIRE(state->H == 0xa5);
	REQUIRE(state->L == 0xf9);
	REQUIRE(state->flag.C == true);
}

//Logical group
TEST_CASE("ANA, XRA and ORA groups") {
	state->A = 0x6f;
	ANA(0x56);
	XRA(0xaa);
	ORA(0x99);
	ANA(0x66);
	XRA(0xbb);
	ORA(0x22);
	REQUIRE(state->A == 0xff);
}

TEST_CASE("CMP, CMP M and CPI") {
	state->flag.Z = false;
	state->flag.S = false;
	state->flag.C = false;

	state->A = 0xf0;
	CMP(0xf0);
	REQUIRE(state->flag.Z == true);
	CMP(0xff);
	REQUIRE(state->flag.S == true);
	REQUIRE(state->flag.C == true);
}

TEST_CASE("RLC and RRC") {
	state->A = 0xc8;
	RLC();
	RLC();
	RLC();
	RRC();
	RRC();
	REQUIRE(state->A == 0x91);
}

TEST_CASE("RAL and RAR") {
	state->flag.C = false;
	state->A = 0xc8;
	RAL();
	RAL();
	RAL();
	REQUIRE(state->A == 0x43);
	RAR();
	RAR();
	REQUIRE(state->A == 0x90);
}

TEST_CASE("CMC and CMA") {
	state->A = 0x44;
	state->flag.C = false;
	CMA();
	REQUIRE(state->A == 0xbb);
	CMC();
	REQUIRE(state->flag.C == true);
}

//Branch group
TEST_CASE("Jump tests") {

}

TEST_CASE("CALL and RET") {
	state->PC = 0x1000;
	state->SP = 0xffff;
	RAM[0x1000] = 0xcd;
	RAM[0x1001] = 0xa;
	RAM[0x1002] = 0xb;
	RAM[0x1003] = 0xc;
	state->memory = RAM;
	uint8_t byte2 = state->memory[state->PC + 1];
	uint8_t byte3 = state->memory[state->PC + 2];

	REQUIRE(state->PC + 2 == 0x1002);
	CALL(byte2, byte3);
	REQUIRE(state->PC == 0x0b0a);
	REQUIRE(state->SP == (0xffff - 2));
	REQUIRE(state->memory[state->SP] == 0x03);
	REQUIRE(state->memory[state->SP + 1] == 0x10);
	RET();
	REQUIRE(state->PC == 0x1003);
}

TEST_CASE("RST 0-7") {
	for (int NNN = 0; NNN <= 7; ++NNN) {
		RST(NNN);
		REQUIRE(state->PC == (8 * NNN));
	}
}

TEST_CASE("PCHL") {
	state->H = 0x7f;
	state->L = 0xf8;
	PCHL();
	REQUIRE(state->PC == 0x7ff8);
}

TEST_CASE("PUSH and POP") {
	state->B = 0x11;
	state->C = 0x44;
	state->SP = 0xffff;
	PUSH(state->B, state->C);
	state->B = 0;
	state->C = 0;
	POP(state->B, state->C);
	REQUIRE(state->C == 0x44);
}

TEST_CASE("PUSH and POP PSW") {
	state->flag.Z = true;
	state->flag.C = false;
	state->flag.P = true;
	state->flag.S = true;
	PUSH_PSW();
	state->flag.Z = false;
	state->flag.C = true;
	state->flag.P = false;
	state->flag.S = false;
	POP_PSW();
	REQUIRE(state->flag.Z == true);
	REQUIRE(state->flag.C == false);
	REQUIRE(state->flag.P == true);
	REQUIRE(state->flag.S == true);
}