#pragma once
#include "cpu.h"

//instructions functions
//MOV and MVI are together
void MOV(uint8_t& dest, const uint8_t& orig) {
	dest = orig;
}

//LXI and LXI SP are diferent, ideally, i want these two as one function
void LXI_SP(const uint8_t& byte2, const uint8_t& byte3) {
	state->SP = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
}

void LXI(uint8_t& regH, uint8_t& regL, const uint8_t& byte2, const uint8_t& byte3) {
	regH = byte3;
	regL = byte2;
}

//load accumulator
void LDA(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
	state->A = state->memory[address];
}

//store accumulator
void STA(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
	state->memory[address] = state->A;
}

void LHLD(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
	state->L = state->memory[address];
	state->H = state->memory[address + 1];
}