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
		uint16_t shifted = ((shift16.status << shift16.offset));
		shift16.result = (shifted & 0xff00) >> 8;
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
		shift16.offset = value & 0x7;
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
	//state->SP += 0x100;
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
	uint16_t result = (uint16_t)state->A + (uint16_t)reg + (int)state->flag.C;
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
	//clear carry and auxiliary carry flags
	state->flag.C = false;
	state->flag.AC = false;
	parity(state->A & 0xff) ? state->flag.P = true : state->flag.P = false;
}

void ORA(const uint8_t& reg) { //ORA, ORA M, ORI
	state->A = state->A | reg;
	(state->A & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(state->A & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	state->A > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(state->A & 0xff) ? state->flag.P = true : state->flag.P = false;
}

void CMP(const uint8_t& reg) { //CMP, CMP M, CPI
	uint8_t result = state->A - reg;
	(state->A == reg) ? state->flag.Z = true : state->flag.Z = false;
	(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	(state->A < reg) ? state->flag.C = true : state->flag.C = false;
	parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
}

void RLC() {
	uint8_t x = state->A;
	state->A = ((x & 0x80) >> 7) | (x << 1);
	(x & 0x80) != 0 ? state->flag.C = true : state->flag.C = false;
}

void RRC() {
	uint8_t x = state->A;
	state->A = ((x & 1) << 7) | (x >> 1);
	state->flag.C = (1 == (x & 1));
}

void RAL() {
	uint8_t before = state->A;
	uint8_t after = state->A << 1;
	after = after | state->flag.C;
	(before & 0x80) != 0 ? state->flag.C = true : state->flag.C = false;
	state->A = after;
}

void RAR() {
	uint8_t before = state->A;
	uint8_t after = state->A >> 1;
	after = after | ((uint8_t)state->flag.C << 7);
	state->flag.C = before & 0x1;
	state->A = after;
}

void CMA() {
	state->A = ~state->A;
}

void CMC() {
	state->flag.C = !state->flag.C;
}

void STC() {
	state->flag.C = true;
}

void JMP(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = byte2 | (byte3 << 8);
	state->PC = address;
}

void JNZ(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.Z) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void JZ(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.Z) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void JNC(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.C) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void JC(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.C) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void JP(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.S) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void JM(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.S) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void JPO(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.P) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void JPE(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.P) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void CALL(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t next = state->PC + 3;
	state->memory[state->SP - 1] = (next >> 8) & 0xff;
	state->memory[state->SP - 2] = (next & 0xff);
	state->SP -= 2;
	state->PC = (byte3 << 8) | (byte2);
}

void CZ(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.Z) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void CNZ(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.Z) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void CP(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.S) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void CM(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.S) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void CC(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.C) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void CNC(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.C) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void CPO(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.P) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void CPE(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.P) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

void RET() {
	state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
	state->SP += 2;
}

void RZ() {
	if (state->flag.Z) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

void RNZ() {
	if (!state->flag.Z) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

void RC() {
	if (state->flag.C) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

void RNC() {
	if (!state->flag.C) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

void RP() {
	if (!state->flag.S) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

void RM() {
	if (state->flag.S) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

void RPE() {
	if (state->flag.P) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

void RPO() {
	if (!state->flag.P) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

void RST(const uint8_t NNN) {
	state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
	state->memory[state->SP - 2] = state->PC & 0xff;
	state->SP -= 2;
	state->PC = (8 * NNN) & 0xff;
}

void PCHL() {
	state->PC = state->pair(state->H, state->L);
}

void PUSH(const uint8_t& regH, const uint8_t& regL) {
	state->memory[state->SP - 1] = regH;
	state->memory[state->SP - 2] = regL;
	state->SP -= 2;
}

void POP(uint8_t& regH, uint8_t& regL) {
	regL = state->memory[state->SP];
	regH = state->memory[state->SP + 1];
	state->SP += 2;
}

void PUSH_PSW() {
	state->memory[state->SP - 1] = state->A;
	uint8_t psw = (state->flag.C |
		(0x01 << 1) |
		(state->flag.P << 2) |
		(0x00 << 3) |
		(state->flag.AC << 4)) |
		(0x00 << 5) |
		(state->flag.Z << 6) |
		(state->flag.S << 7);
	state->memory[state->SP - 2] = psw;
	state->SP -= 2;
}

void POP_PSW() {
	uint8_t psw = state->memory[state->SP];
	state->flag.C = ((psw & 0x01) != 0);
	state->flag.P = ((psw & 0x04) != 0);
	state->flag.AC = ((psw & 0x10) != 0);
	state->flag.Z = ((psw & 0x40) != 0);
	state->flag.S = ((psw & 0x80) != 0);
	state->SP += 2;
}

void XTHL() {
	std::swap(state->H, state->memory[state->SP + 1]);
	std::swap(state->L, state->memory[state->SP]);

	state->HL_pair = state->pair(state->H, state->L);
}

void SPHL() {
	state->SP = state->HL_pair = state->pair(state->H, state->L);
}