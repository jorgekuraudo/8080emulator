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
		shift16.status = (shift16.status & 0x00ff) | ((uint16_t)value << 8);
		break;
	}
	}
}

//instructions functions
//MOV and MVI are together
inline void MOV(uint8_t& dest, const uint8_t& orig) {
	dest = orig;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

//LXI and LXI SP are diferent, ideally, i want these two as one function
inline void LXI_SP(const uint8_t& byte2, const uint8_t& byte3) {
	state->SP = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
}

inline void LXI(uint8_t& regH, uint8_t& regL, const uint8_t& byte2, const uint8_t& byte3) {
	regH = byte3;
	regL = byte2;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

//load accumulator
inline void LDA(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
	state->A = state->memory[address];
}

//store accumulator
inline void STA(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
	state->memory[address] = state->A;
}

inline void LHLD(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
	state->L = state->memory[address];
	state->H = state->memory[address + 0x01];
	state->HL_pair = state->pair(state->H, state->L);
}

inline void SHLD(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = (uint16_t)(byte3 << 8) | (uint16_t)byte2;
	state->memory[address] = state->L;
	state->memory[address + 0x01] = state->H;
}

inline void LDAX(const uint16_t& reg_pair) { //only BC and DE pairs
	state->A = state->memory[reg_pair];
}

inline void STAX(const uint16_t& reg_pair) { //only BC and DE pairs
	state->memory[reg_pair] = state->A;
}

inline void XCHG() {
	std::swap(state->H, state->D);
	std::swap(state->L, state->E);

	state->HL_pair = state->pair(state->H, state->L);
	state->DE_pair = state->pair(state->D, state->E);
}

inline void ADD(const uint8_t& reg) {
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

inline void ADC(const uint8_t& reg) {
	uint16_t result = (uint16_t)state->A + (uint16_t)reg + (int)state->flag.C;
	(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	result > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;

	state->A = result & 0xff; //add result to accumulator (only the 8 low bits)
}

inline void SUB(const uint8_t& reg) {
	uint16_t result = (uint16_t)state->A - (uint16_t)reg;
	(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	result > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;

	state->A = result & 0xff; //add result to accumulator (only the 8 low bits)
}

inline void SBB(const uint8_t& reg) {
	uint16_t result = (uint16_t)state->A - (uint16_t)reg - state->flag.C;
	(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	result > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;

	state->A = result & 0xff; //add result to accumulator (only the 8 low bits)
}

inline void INR(uint8_t& reg) {
	++reg;
	reg == 0 ? state->flag.Z = true : state->flag.Z = false;
	(reg & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	parity(reg & 0xff) ? state->flag.P = true : state->flag.P = false;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

inline void DCR(uint8_t& reg) {
	--reg;
	reg == 0 ? state->flag.Z = true : state->flag.Z = false;
	(reg & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	parity(reg & 0xff) ? state->flag.P = true : state->flag.P = false;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

inline void INX(uint8_t& regH, uint8_t& regL) {
	uint16_t pair = state->pair(regH, regL);
	++pair;
	regL = pair & 0xff;
	regH = (pair >> 8) & 0xff;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

inline void DCX(uint8_t& regH, uint8_t& regL) {
	uint16_t pair = state->pair(regH, regL);
	--pair;
	regL = pair & 0xff;
	regH = (pair >> 8) & 0xff;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

inline void DAD(const uint8_t& regH, const uint8_t& regL) {
	uint16_t pair = state->pair(regH, regL);
	uint32_t result = state->HL_pair + pair;
	result > 0xffff ? state->flag.C = true : state->flag.C = false;

	state->L = result & 0xff;
	state->H = (result >> 8) & 0xff;
	state->HL_pair = state->pair(state->H, state->L);
}

inline void ANA(const uint8_t& reg) { //ANA, ANA M, ANI
	state->A = state->A & reg;
	(state->A & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(state->A & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	state->A > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(state->A & 0xff) ? state->flag.P = true : state->flag.P = false;
}

inline void XRA(const uint8_t& reg) { //XRA, XRA M, XRI
	state->A = state->A ^ reg;
	(state->A & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(state->A & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	//clear carry and auxiliary carry flags
	state->flag.C = false;
	state->flag.AC = false;
	parity(state->A & 0xff) ? state->flag.P = true : state->flag.P = false;
}

inline void ORA(const uint8_t& reg) { //ORA, ORA M, ORI
	state->A = state->A | reg;
	(state->A & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
	(state->A & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	state->A > 0xff ? state->flag.C = true : state->flag.C = false;
	parity(state->A & 0xff) ? state->flag.P = true : state->flag.P = false;
}

inline void CMP(const uint8_t& reg) { //CMP, CMP M, CPI
	uint8_t result = state->A - reg;
	(state->A == reg) ? state->flag.Z = true : state->flag.Z = false;
	(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
	(state->A < reg) ? state->flag.C = true : state->flag.C = false;
	parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
}

inline void RLC() {
	uint8_t x = state->A;
	state->A = ((x & 0x80) >> 7) | (x << 1);
	(x & 0x80) != 0 ? state->flag.C = true : state->flag.C = false;
}

inline void RRC() {
	uint8_t x = state->A;
	state->A = ((x & 1) << 7) | (x >> 1);
	state->flag.C = (1 == (x & 1));
}

inline void RAL() {
	uint8_t before = state->A;
	uint8_t after = state->A << 1;
	after = after | (int)state->flag.C;
	(before & 0x80) != 0 ? state->flag.C = true : state->flag.C = false;
	state->A = after;
}

inline void RAR() {
	uint8_t before = state->A;
	uint8_t after = state->A >> 1;
	after = after | ((uint8_t)state->flag.C << 7);
	state->flag.C = before & 0x1;
	state->A = after;
}

inline void CMA() {
	state->A = ~state->A;
}

inline void CMC() {
	state->flag.C = !state->flag.C;
}

inline void STC() {
	state->flag.C = true;
}

inline void JMP(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t address = byte2 | (byte3 << 8);
	state->PC = address;
}

inline void JNZ(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.Z) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void JZ(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.Z) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void JNC(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.C) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void JC(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.C) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void JP(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.S) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void JM(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.S) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void JPO(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.P) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void JPE(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.P) {
		JMP(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void CALL(const uint8_t& byte2, const uint8_t& byte3) {
	uint16_t next = state->PC + 3;
	state->memory[state->SP - 1] = (next >> 8) & 0xff;
	state->memory[state->SP - 2] = (next & 0xff);
	state->SP -= 2;
	state->PC = (byte3 << 8) | (byte2);
}

inline void CZ(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.Z) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void CNZ(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.Z) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void CP(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.S) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void CM(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.S) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void CC(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.C) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void CNC(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.C) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void CPO(const uint8_t& byte2, const uint8_t& byte3) {
	if (!state->flag.P) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void CPE(const uint8_t& byte2, const uint8_t& byte3) {
	if (state->flag.P) {
		CALL(byte2, byte3);
	}
	else {
		state->PC += 3;
	}
}

inline void RET() {
	state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
	state->SP += 2;
}

inline void RZ() {
	if (state->flag.Z) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

inline void RNZ() {
	if (!state->flag.Z) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

inline void RC() {
	if (state->flag.C) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

inline void RNC() {
	if (!state->flag.C) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

inline void RP() {
	if (!state->flag.S) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

inline void RM() {
	if (state->flag.S) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

inline void RPE() {
	if (state->flag.P) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

inline void RPO() {
	if (!state->flag.P) {
		RET();
	}
	else {
		state->PC += 1;
	}
}

inline void RST(const uint8_t NNN) {
	state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
	state->memory[state->SP - 2] = state->PC & 0xff;
	state->SP -= 2;
	state->PC = (8 * NNN) & 0xff;
}

inline void PCHL() {
	state->PC = state->pair(state->H, state->L);
}

inline void PUSH(const uint8_t& regH, const uint8_t& regL) {
	state->memory[state->SP - 1] = regH;
	state->memory[state->SP - 2] = regL;
	state->SP -= 2;
}

inline void POP(uint8_t& regH, uint8_t& regL) {
	regL = state->memory[state->SP];
	regH = state->memory[state->SP + 1];
	state->SP += 2;
	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
}

inline void PUSH_PSW() {
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

inline void POP_PSW() {
	uint8_t psw = state->memory[state->SP];
	state->A = state->memory[state->SP + 1];
	state->flag.C = ((psw & 0x01) != 0);
	state->flag.P = ((psw & 0x04) != 0);
	state->flag.AC = ((psw & 0x10) != 0);
	state->flag.Z = ((psw & 0x40) != 0);
	state->flag.S = ((psw & 0x80) != 0);
	state->SP += 2;
}

inline void XTHL() {
	std::swap(state->H, state->memory[state->SP + 1]);
	std::swap(state->L, state->memory[state->SP]);

	state->HL_pair = state->pair(state->H, state->L);
}

inline void SPHL() {
	state->SP = state->HL_pair = state->pair(state->H, state->L);
}