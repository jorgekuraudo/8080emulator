#pragma once
#include "cpu.h"
#include "inout.h"

//mIN and mOUT deal with system's I/O
void mIN(uint8_t port) {
	switch (port)
	{
	case 1:
	{
		state->A = bus.port1;
		break;
	}
	case 2:
	{
		state->A = bus.port2;
		break;
	}
	case 3:
	{
		state->A = shift16.result;
		break;
	}
	}
}
void mOUT(uint8_t port, uint8_t value) {
	switch (port)
	{
	case 2:
	{
		uint8_t offset = value;
		uint16_t shifted = ((shift16.status << offset));
		shift16.result = (shifted & 0xff00) >> 8;
		break;
	}
	case 4:
	{
		shift16.status = shift16.status >> 8;
		shift16.status = (shift16.status & 0x00ff) | (value << 8);
		break;
	}
	}
}

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
	state->H = state->memory[address + 0x01];
	state->HL_pair = state->pair(state->H, state->L);
}

void SHLD(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
	state->memory[address] = state->L;
	state->memory[address + 0x01] = state->H;
}

void LDAX(const uint16_t& reg_pair) { //only BC and DE pairs
	state->A = state->memory[reg_pair];
}

void STAX(const uint16_t& reg_pair) { //only BC and DE pairs
	state->memory[reg_pair] = state->A;
}

void XCHG() {
	std::swap(state->H, state->D);
	std::swap(state->L, state->E);

	state->HL_pair = state->pair(state->H, state->L);
	state->DE_pair = state->pair(state->D, state->E);
}

void ADD(const uint8_t& reg) {
	uint16_t result = (uint16_t)state->A + (uint16_t)reg;
	//flags:
	//Zero flag
	(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	//Sign flag
	(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	//Carry flag
	result > 0xff ? state->flag.C = true : state->flag.C = false;
	//Parity flag
	parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;

	state->A = result & 0xff; //add result to accumulator (only the 8 low bits)
}

void ADC(const uint8_t& reg) {
	uint16_t result = (uint16_t)state->A + (uint16_t)reg + state->flag.C;
	(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	result > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;

	state->A = result & 0xff; //add result to accumulator (only the 8 low bits)
}

void SUB(const uint8_t& reg) {
	uint16_t result = (uint16_t)state->A - (uint16_t)reg;
	(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	result > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;

	state->A = result & 0xff; //add result to accumulator (only the 8 low bits)
}

void SBB(const uint8_t& reg) {
	uint16_t result = (uint16_t)state->A - (uint16_t)reg - state->flag.C;
	(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	result > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;

	state->A = result & 0xff; //add result to accumulator (only the 8 low bits)
}

void INR(uint8_t& reg) {
	++reg;
	reg == 0 ? state->flag.Z = true : state->flag.Z = false;
	(reg & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	parity(reg & 0xff) ? state->flag.P = true : state->flag.P = false;
}

void DCR(uint8_t& reg) {
	--reg;
	reg == 0 ? state->flag.Z = true : state->flag.Z = false;
	(reg & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	parity(reg & 0xff) ? state->flag.P = true : state->flag.P = false;
}

void INX(uint8_t& regH, uint8_t& regL) {
	uint16_t pair = state->pair(regH, regL);
	++pair;
	regL = pair & 0xff;
	regH = (pair >> 8) & 0xff;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

void DCX(uint8_t& regH, uint8_t& regL) {
	uint16_t pair = state->pair(regH, regL);
	--pair;
	regL = pair & 0xff;
	regH = (pair >> 8) & 0xff;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

void DAD(const uint8_t& regH, const uint8_t& regL) {
	uint16_t pair = state->pair(regH, regL);
	uint32_t result = state->HL_pair + pair;
	result > 0xffff ? state->flag.C = true : state->flag.C = false;

	state->L = result & 0xff;
	state->H = (result >> 8) & 0xff;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

void ANA(const uint8_t& reg) { //ANA, ANA M, ANI
	state->A = state->A & reg;
	(state->A & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(state->A & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	state->A > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(state->A & 0xff) ? state->flag.P = true : state->flag.P = false;
}

void XRA(const uint8_t& reg) { //XRA, XRA M, XRI
	state->A = state->A ^ reg;
	(state->A & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(state->A & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	state->A > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(state->A & 0xff) ? state->flag.P = true : state->flag.P = false;
}

void ORA(const uint8_t& reg) { //ORA, ORA M, ORI
	state->A = state->A | reg;
	(state->A & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(state->A & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	state->A > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(state->A & 0xff) ? state->flag.P = true : state->flag.P = false;
}