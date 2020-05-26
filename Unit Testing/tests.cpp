#define CATCH_CONFIG_MAIN
#include "/Users/jmm_1/Desktop/invadersrom/catch.hpp"
#include "../8080emulator/cpu.h"
#include "../8080emulator/instructions.h"

uint8_t RAM[0xfff]{}; //to test memory access

TEST_CASE("Register pair formation") {
	state->B = 0x12;
	state->C = 0x34;
	state->BC_pair = state->pair(state->B, state->C);
	REQUIRE(state->BC_pair == 0x1234);
}

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
		REQUIRE(RAM[1] == state->A);
	}
}

TEST_CASE("LHLD test") {
	RAM[0x1006] = 0x34;
	RAM[0x1007] = 0x12;
	state->memory = RAM;
	uint8_t byte2 = 0x06;
	uint8_t byte3 = 0x10;
	LHLD(byte2, byte3);
	REQUIRE(state->HL_pair == 0x1234);
}