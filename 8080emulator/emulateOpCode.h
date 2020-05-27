#pragma once
#include <iostream>
#include <chrono>
#include <fstream>
#include "cpu.h"
#include "instructions.h"

std::fstream logFile("C:/Users/jmm_1/Desktop/invadersrom/log.txt", std::ios::out);

//turn on/off debugging
bool gDebug{ false };

//system interrupt
void interrupt(State8080* state, int num) {
	state->memory[state->SP - 1] = ((state->PC - 1) >> 8) & 0xff;
	state->memory[state->SP - 2] = (state->PC - 1) & 0xff;
	state->SP -= 2;
	state->PC = (8 * num);
}

void emulateOpCode(State8080* state) {
	
	uint8_t* opcode = &state->memory[state->PC];
	
	if (state->PC == 0x87)
	{
		logFile << "===========return from interrupt===========\n";
	}

	//statements for debugging
	state->count;

	state->HL_pair = state->pair(state->H, state->L);
	state->BC_pair = state->pair(state->B, state->C);
	state->DE_pair = state->pair(state->D, state->E);
	
	//printf( "C: %d\tI: %x  \tO: %x \tSP: %x \tBC: %x \tDE: %x \tHL: %x \tA: %x\t ZSPC: %d%d%d%d  %x \n", state->count, (int)state->PC, *opcode, state->SP, BC_pair, DE_pair, HL_pair, state->A, state->flag.Z, state->flag.S, state->flag.P, state->flag.C, state->memory[0xe602]);
	/*printf("SP(M): 0: %x(%x) 1: %x(%x) 2: %x(%x) 3: %x(%x)\n", state->SP, state->memory[state->SP],
		state->SP + 1, state->memory[state->SP + 1],
		state->SP + 2, state->memory[state->SP + 2],
		state->SP + 3, state->memory[state->SP + 3]);*/
	/*if (state->count % 1000000 == 0) {
		printf("COUNT: %d\tINST: %x\tOPCODE: %x\tSP: %x\tBC: %x\tDE: %x\tHL: %x\tA: %x\t ZSPC: %d%d%d%d\n", state->count, (int)state->PC, *opcode, state->SP, BC_pair, DE_pair, HL_pair, state->A, state->flag.Z, state->flag.S, state->flag.P, state->flag.C);
	}*/
	/*if ((shift16.status != 0 || shift16.result != 0) && (*opcode == 0xdb || *opcode == 0xd3)) {
		printf("CNT: %d\tINS: %x   \tOPCODE: %x\tA: %x\t%x\t\t%x\n", state->count, (int)state->PC, *opcode, state->A, shift16.status, shift16.result);
	}*/
	/*if (state->PC = 0x1439) {
		printf("CNT: %d\tINS: %x   \tOPCODE: %x\tA: %x\t%x\t\t%x\n", state->count, (int)state->PC, *opcode, state->A, shift16.status, shift16.result);
	}*/

	//printf("COUNT: %d\tINST: %x   \tOPCODE: %x\tSP: %x\tBC: %x\tDE: %x\tHL: %x\tA: %x\t ZSPC: %d%d%d%d\n", state->count, (int)state->PC, *opcode, state->SP, BC_pair, DE_pair, HL_pair, state->A, state->flag.Z, state->flag.S, state->flag.P, state->flag.C);

	//if (state->SP < 0x23DE)
	//{
	//	/*printf("------------------------------------------------------------------------\n");
	//	printf("Instruction number: %d\n", state->count);
	//	printf("Memory address: %x\n", state->PC);
	//	printf("Intruction: %x\n", *opcode);
	//	printf("Stack pointer: %x\t stack pointer content: %x\n", (int)state->SP, state->memory[state->SP]);
	//	printf("Registers info: BC: %x   \tDE: %x   \tHL: %x   \tA: %x\n", BC_pair, DE_pair, HL_pair, state->A);
	//	printf("Flags: ZSPC: %d%d%d%d\n", state->flag.Z, state->flag.S, state->flag.P, state->flag.C);*/
	//	printf("COUNT: %d\tINST: %x   \tOPCODE: %x\tSP: %x\tBC: %x\tDE: %x\tHL: %x\tA: %x\t ZSPC: %d%d%d%d\n", state->count, (int)state->PC, *opcode, state->SP, BC_pair, DE_pair, HL_pair, state->A, state->flag.Z, state->flag.S, state->flag.P, state->flag.C);
	//	//printf("%x\n", state->PC);
	//	/*printf("\t\t\t\t\t   SP(M): 0: %x(%x)\t 1: %x(%x)\t 2: %x(%x)\t 3: %x(%x)\n", state->SP, state->memory[state->SP],
	//		state->SP + 1, state->memory[state->SP + 1],
	//		state->SP + 2, state->memory[state->SP + 2],
	//		state->SP + 3, state->memory[state->SP + 3]);*/
	//}

	//logFile << std::dec << "count: " << state->count
	//	<< "\taddr: " << std::hex << state->PC
	//	<< "   \tOPCODE: " << (int)*opcode
	//	<< "\tSP: " << state->SP
	//	<< "   \tBC: " << BC_pair
	//	<< "   \tDE: " << DE_pair
	//	<< "   \tHL: " << HL_pair
	//	<< "   \tA: " << (int)state->A
	//	<< "\tZSPC: " << state->flag.Z << state->flag.S << state->flag.P << state->flag.C
	//	/*<< "  \t(HL): " << (int)state->memory[HL_pair]*/
	//	<< "\n";

	switch (*opcode)
	{
	case 0x0: break; //NOP
	case 0x1:		//LXI B,D16
		LXI(state->B, state->C, opcode[1], opcode[2]);
		state->PC += 2;
		break;
	case 0x2:	//	STAX B
	{
		STAX(state->BC_pair);
		break;
	}
	case 0x3:		//INX B
	{
		INX(state->B, state->C);
		break;
	}
	case 0x4:		//INR B
	{
		INR(state->B);
		break;
	}
	case 0x5:		//DCR B
	{
		DCR(state->B);
		break;
	}
	case 0x6:		//MVI B,D8
		MOV(state->B, opcode[1]);
		state->PC += 1;
		break;
	case 0x7:		//RLC
	{
		uint8_t x = state->A;
		state->A = ((x & 0x80) >> 7) | (x << 1);
		(x & 0x80) != 0 ? state->flag.C = true : state->flag.C = false;
		break;

	}
	case 0x8: std::cout << "0x8 -" << std::endl; break;
	case 0x9:		//DAD B
	{
		DAD(state->B, state->C);
		break;
	}
	case 0xa:		//LDAX B
	{
		LDAX(state->BC_pair);
		break;
	}
	case 0xb:		//DCX B
	{
		DCX(state->B, state->C);
		break;
	}
	case 0xc:		//INR C
	{
		INR(state->C);
		break;
	}
	case 0xd:		//DCR C
	{
		DCR(state->C);
		break;
	}
	case 0xe:		//MVI C,D8
		MOV(state->C, opcode[1]);
		state->PC += 1;
		break;
	case 0xf:		//RRC
	{
		uint8_t x = state->A;
		state->A = ((x & 1) << 7) | (x >> 1);
		state->flag.C = (1 == (x & 1));
		break;
	}
	case 0x10: std::cout << "0x10 -" << std::endl; break;
	case 0x11:		//LXI D, D16
		LXI(state->D, state->E, opcode[1], opcode[2]);
		state->PC += 2;
		break;
	case 0x12:	//	STAX D
	{
		STAX(state->DE_pair);
		break;
	}
	case 0x13:		//INX D
	{
		INX(state->D, state->E);
		break;
	}
	case 0x14:		//INR D
	{
		INR(state->D);
		break;
	}
	case 0x15:		//DCR D
	{
		DCR(state->D);
		break;
	}
	case 0x16:		//MVI D,D8
		MOV(state->D, opcode[1]);
		state->PC += 1;
		break;
	case 0x17:		//RAL
	{
		uint8_t before = state->A;
		uint8_t after = state->A << 1;
		after = after | (state->flag.C) & 1;
		state->flag.C = before & 0x80;
		state->A = after;
		break;

	}
	case 0x18: std::cout << "0x18 -" << std::endl; break;
	case 0x19:		//DAD D
	{
		DAD(state->D, state->E);
		break;
	}
	case 0x1a:		//LDAX D
	{
		LDAX(state->DE_pair);
		break;
	}
	case 0x1b:		//DCX D
	{
		DCX(state->D, state->E);
		break;
	}
	case 0x1c:		//INR E
	{
		INR(state->E);
		break;
	}
	case 0x1d:		//DCR E
	{
		DCR(state->E);
		break;
	}
	case 0x1e:		//MVI E,D8
		MOV(state->E, opcode[1]);
		state->PC += 1;
		break;
	case 0x1f:		//RCL
	{
		uint8_t before = state->A;
		uint8_t after = state->A >> 1;
		after = after | (before & 0x80) & 0x80;
		state->flag.C = before & 0x1;
		state->A = after;
		break;

	}
	case 0x20: std::cout << "0x20 -" << std::endl; break;
	case 0x21:		//LXI H, D16
		LXI(state->H, state->L, opcode[1], opcode[2]);
		state->PC += 2;
		break;
	case 0x22:		//SHLD adr
	{
		SHLD(opcode[1], opcode[2]);
		state->PC += 2;
		break;
	}
	case 0x23:		//INX H
	{
		INX(state->H, state->L);
		break;
	}
	case 0x24:		//INR H
	{
		INR(state->H);
		break;
	}
	case 0x25:		//DCR H
	{
		DCR(state->H);
		break;
	}
	case 0x26:		//MVI H,D8
		MOV(state->H, opcode[1]);
		state->PC += 1;
		break;
	case 0x27:		//DAA
	{
		if ((state->A & 0xf) > 9 || state->flag.AC) {
			state->A += 6;
		}
		else if (((state->A >> 4) & 0xf) > 9 || state->flag.C) {
			state->A += (6 << 4);
		}
		break;
	}
	case 0x28:
		printf("COUNT: %d\tINST: %x   \tOPCODE: %x\tSP: %x\tBC: %x\tDE: %x\tHL: %x\tA: %x\t ZSPC: %d%d%d%d\n", state->count, (int)state->PC, *opcode, state->SP, state->BC_pair, state->DE_pair, state->HL_pair, state->A, state->flag.Z, state->flag.S, state->flag.P, state->flag.C);
		break;
	case 0x29:		//DAD H
	{
		DAD(state->H, state->L);
	}
	break;
	case 0x2a:		//LHLD adr
	{
		LHLD(opcode[1], opcode[2]);
		state->PC += 2;
		break;
	}
	case 0x2b:		//DCX H
	{
		DCX(state->H, state->L);
		break;
	}
	case 0x2c:		//INR L
	{
		INR(state->L);
		break;
	}
	case 0x2d:		//DCR L
	{
		DCR(state->L);
		break;
	}
	case 0x2e:		//MVI L,D8
		MOV(state->L, opcode[1]);
		state->PC += 1;
		break;
	case 0x2f:		//CMA
		state->A = ~state->A;
		break;
	case 0x30: std::cout << "0x30 -" << std::endl; break;
	case 0x31:		//LXI SP, D16
		LXI_SP(opcode[1], opcode[2]);
		state->PC += 2;
		break;
	case 0x32:		//STA
	{
		STA(opcode[1], opcode[2]);
		state->PC += 2;
		break;
	}
	case 0x33:		//INX SP
	{
		++state->SP;
		break;
	}
	case 0x34:		//INR M
	{
		INR(state->memory[state->HL_pair]);
		break;

	}
	case 0x35:		//DCR M
	{
		DCR(state->memory[state->HL_pair]);
		break;

	}
	case 0x36:		//MVI M,D8
	{
		MOV(state->memory[state->HL_pair], opcode[1]);
		state->PC += 1;
		break;
	}
	case 0x37:		//STC
	{
		state->flag.C = true;
		break;
	}
	case 0x38: std::cout << "0x38 -" << std::endl; break;
	case 0x39:		//DAD SP
	{
		DAD((state->SP >> 8) & 0xff, state->SP & 0xff);
		break;
	}
	case 0x3a:		//LDA addr
	{
		LDA(opcode[1], opcode[2]);
		state->PC += 2;
		break;
	}
	case 0x3b:		//DCX SP
	{
		--state->SP;
		break;
	}
	case 0x3c: //INR A
	{
		INR(state->A);
		break;
	}
	case 0x3d: //DCR A
	{
		DCR(state->A);
		break;
	}
	case 0x3e:		//MVI A,D8
		MOV(state->A, opcode[1]);
		state->PC += 1;
		break;
	case 0x3f:		//CMC
	{
		state->flag.C = false;
		break;
	}
	case 0x40:		//MOV B,B
	{
		MOV(state->B, state->B);
		break;
	}
	case 0x41:		//MOV B,C
	{
		MOV(state->B, state->C);
		break;
	}
	case 0x42:		//MOV B,D
	{
		MOV(state->B, state->D);
		break;
	}
	case 0x43:		//MOV B,E
	{
		MOV(state->B, state->E);
		break;
	}
	case 0x44:		//MOV B,H
	{
		MOV(state->B, state->H);
		break;
	}
	case 0x45:		//MOV B,L
	{
		MOV(state->B, state->L);
		break;
	}
	case 0x46:		//MOV B,M
	{
		MOV(state->B, state->memory[state->HL_pair]);
		break;
	}
	case 0x47:		//MOV B,A
	{
		MOV(state->B, state->A);
		break;
	}
	case 0x48:		//MOV C,B
	{
		MOV(state->C, state->B);
		break;
	}
	case 0x49:		//MOV C,C
	{
		MOV(state->C, state->C);
		break;
	}
	case 0x4a:		//MOV C,D
	{
		MOV(state->C, state->D);
		break;
	}
	case 0x4b:		//MOV C,E
	{
		MOV(state->C, state->E);
		break;
	}
	case 0x4c:		//MOV C,H
	{
		MOV(state->C, state->H);
		break;
	}
	case 0x4d:		//MOV C,L
	{
		MOV(state->C, state->L);
		break;
	}
	case 0x4e:		//MOV C,M
	{
		MOV(state->C, state->memory[state->HL_pair]);
		break;
	}
	case 0x4f:		//MOV C,A
	{
		MOV(state->C, state->A);
		break;
	}
	case 0x50:		//MOV D,B
	{
		MOV(state->D, state->B);
		break;
	}
	case 0x51:		//MOV D,C
	{
		MOV(state->D, state->C);
		break;
	}
	case 0x52:		//MOV D,D
	{
		MOV(state->D, state->D);
		break;
	}
	case 0x53:		//MOV D,E
	{
		MOV(state->D, state->E);
		break;
	}
	case 0x54:		//MOV D,H
	{
		MOV(state->D, state->H);
		break;
	}
	case 0x55:		//MOV D,L
	{
		MOV(state->D, state->L);
		break;
	}
	case 0x56:		//MOV D,M
	{
		MOV(state->D, state->memory[state->HL_pair]);
		break;
	}
	case 0x57:		//MOV D,A
	{
		MOV(state->D, state->A);
		break;
	}
	case 0x58:		//MOV E,B
	{
		MOV(state->E, state->B);
		break;
	}
	case 0x59:		//MOV E,C
	{
		MOV(state->E, state->C);
		break;
	}
	case 0x5a:		//MOV E,D
	{
		MOV(state->E, state->D);
		break;
	}
	case 0x5b:		//MOV E,E
	{
		MOV(state->E, state->E);
		break;
	}
	case 0x5c:		//MOV E,H
	{
		MOV(state->E, state->H);
		break;
	}
	case 0x5d:		//MOV E,L
	{
		MOV(state->E, state->L);
		break;
	}
	case 0x5e:		//MOV E,M
	{
		MOV(state->E, state->memory[state->HL_pair]);
		break;
	}
	case 0x5f:		//MOV E,A
	{
		MOV(state->E, state->A);
		break;
	}
	case 0x60:		//MOV H,B
	{
		MOV(state->H, state->B);
		break;
	}
	case 0x61:		//MOV H,C
	{
		MOV(state->H, state->C);
		break;
	}
	case 0x62:		//MOV H,D
	{
		MOV(state->H, state->D);
		break;
	}
	case 0x63:		//MOV H,E
	{
		MOV(state->H, state->E);
		break;
	}
	case 0x64:		//MOV H,H
	{
		MOV(state->H, state->H);
		break;
	}
	case 0x65:		//MOV H,L
	{
		MOV(state->H, state->L);
		break;
	}
	case 0x66:		//MOV H,M
	{
		MOV(state->H, state->memory[state->HL_pair]);
		break;
	}
	case 0x67:		//MOV H,A
	{
		MOV(state->H, state->A);
		break;
	}
	case 0x68:		//MOV L,B
	{
		MOV(state->L, state->B);
		break;
	}
	case 0x69:		//MOV L,C
	{
		MOV(state->L, state->C);
		break;
	}
	case 0x6a:		//MOV L,D
	{
		MOV(state->L, state->D);
		break;
	}
	case 0x6b:		//MOV L,E
	{
		MOV(state->L, state->E);
		break;
	}
	case 0x6c:		//MOV L,H
	{
		MOV(state->L, state->H);
		break;
	}
	case 0x6d:		//MOV L,L
	{
		MOV(state->L, state->L);
		break;
	}
	case 0x6e:		//MOV L,M
	{
		MOV(state->L, state->memory[state->HL_pair]);
		break;
	}
	case 0x6f:		//MOV L,A
	{
		MOV(state->L, state->A);
		break;
	}
	case 0x70:		//MOV M,B
	{
		MOV(state->memory[state->HL_pair], state->B);
		break;
	}
	case 0x71:		//MOV M,C
	{
		MOV(state->memory[state->HL_pair], state->C);
		break;
	}
	case 0x72:		//MOV M,D
	{
		MOV(state->memory[state->HL_pair], state->D);
		break;
	}
	case 0x73:		//MOV M,E
	{
		MOV(state->memory[state->HL_pair], state->E);
		break;
	}
	case 0x74:		//MOV M,H
	{
		MOV(state->memory[state->HL_pair], state->H);
		break;
	}
	case 0x75:		//MOV M,L
	{
		MOV(state->memory[state->HL_pair], state->L);
		break;
	}
	case 0x76:		//HLT
	{
		break;
	}
	case 0x77:		//MOV M,A
	{
		MOV(state->memory[state->HL_pair], state->A);
		break;
	}
	case 0x78:		//MOV A,B
	{
		MOV(state->A, state->B);
		break;
	}
	case 0x79:		//MOV A,C
	{
		MOV(state->A, state->C);
		break;
	}
	case 0x7a:		//MOV A,D
	{
		MOV(state->A, state->D);
		break;
	}
	case 0x7b:		//MOV A,E
	{
		MOV(state->A, state->E);
		break;
	}
	case 0x7c:		//MOV A,H
	{
		MOV(state->A, state->H);
		break;
	}
	case 0x7d:		//MOV A,L
	{
		MOV(state->A, state->L);
		break;
	}
	case 0x7e:		//MOV A,M
	{
		MOV(state->A, state->memory[state->HL_pair]);
		break;
	}
	case 0x7f:		//MOV A,A
	{
		MOV(state->A, state->A);
		break;
	}
	case 0x80:		//ADD B
	{
		ADD(state->B);
		break;
	}
	case 0x81:		//ADD C
	{
		ADD(state->C);
		break;
	}
	case 0x82:		//ADD D
	{
		ADD(state->D);
		break;
	}
	case 0x83:		//ADD E
	{
		ADD(state->E);
		break;
	}
	case 0x84:		//ADD H
	{
		ADD(state->H);
		break;
	}
	case 0x85:		//ADD L
	{
		ADD(state->L);
		break;
	}
	case 0x86:		//ADD M
	{
		ADD(state->memory[state->HL_pair]);
		break;
	}
	case 0x87:		//ADD A
	{
		ADD(state->A);
		break;
	}
	case 0x88:		//ADC B
	{
		ADC(state->B);
		break;
	}
	case 0x89:		//ADC C
	{
		ADC(state->C);
	}
	case 0x8a:		//ADC D
	{
		ADC(state->D);
		break;
	}
	case 0x8b:		//ADC E
	{
		ADC(state->E);
		break;
	}
	case 0x8c:		//ADC H
	{
		ADC(state->H);
		break;
	}
	case 0x8d:		//ADC L
	{
		ADC(state->L);
		break;
	}
	case 0x8e:		//ADC M
	{
		ADC(state->memory[state->HL_pair]);
		break;
	}
	case 0x8f:		//ADC A
	{
		ADC(state->A);
		break;
	}
	case 0x90:		//SUB B
	{
		SUB(state->B);
		break;
	}
	case 0x91:		//SUB C
	{
		SUB(state->C);
		break;
	}
	case 0x92:		//SUB D
	{
		SUB(state->D);
		break;
	}
	case 0x93:		//SUB E
	{
		SUB(state->E);
		break;
	}
	case 0x94:		//SUB H
	{
		SUB(state->H);
		break;
	}
	case 0x95:		//SUB L
	{
		SUB(state->L);
		break;
	}
	case 0x96:		//SUB M
	{
		SUB(state->memory[state->HL_pair]);
		break;
	}
	case 0x97:		//SUB A
	{
		SUB(state->A);
		break;
	}
	case 0x98:		//SBB B
	{
		SBB(state->B);
		break;
	}
	case 0x99:		//SBB C
	{
		SBB(state->C);
		break;
	}
	case 0x9a:		//SBB D
	{
		SBB(state->D);
		break;
	}
	case 0x9b:		//SBB E
	{
		SBB(state->E);
		break;
	}
	case 0x9c:		//SBB H
	{
		SBB(state->H);
		break;
	}
	case 0x9d:		//SBB L
	{
		SBB(state->L);
		break;
	}
	case 0x9e:		//SBB M
	{
		SBB(state->memory[state->HL_pair]);
		break;
	}
	case 0x9f:		//SBB A
	{
		SBB(state->A);
		break;
	}
	case 0xa0:		//ANA B
	{
		ANA(state->B);
		break;
	}
	case 0xa1:		//ANA C
	{
		ANA(state->C);
		break;
	}
	case 0xa2:		//ANA D
	{
		ANA(state->D);
		break;
	}
	case 0xa3:		//ANA E
	{
		ANA(state->E);
		break;
	}
	case 0xa4:		//ANA H
	{
		ANA(state->H);
		break;
	}
	case 0xa5:		//ANA L
	{
		ANA(state->L);
		break;
	}
	case 0xa6:		//ANA M
	{
		ANA(state->memory[state->HL_pair]);
		break;
	}
	case 0xa7:		//ANA A
	{
		ANA(state->A);
		break;
	}
	case 0xa8:		//XRA B
	{
		XRA(state->B);
		break;
	}
	case 0xa9:		//XRA C
	{
		XRA(state->C);
		break;
	}
	case 0xaa:		//XRA D
	{
		XRA(state->D);
		break;
	}
	case 0xab:		//XRA E
	{
		XRA(state->E);
		break;
	}
	case 0xac:		//XRA H
	{
		XRA(state->H);
		break;
	}
	case 0xad:		//XRA L
	{
		XRA(state->L);
		break;
	}
	case 0xae:		//XRA M
	{
		XRA(state->memory[state->HL_pair]);
		break;
	}
	case 0xaf:		//XRA A
	{
		XRA(state->A);
		break;
	}
	case 0xb0:		//ORA B
	{
		ORA(state->B);
		break;
	}
	case 0xb1:		//ORA C
	{
		XRA(state->C);
		break;
	}
	case 0xb2:		//ORA D
	{
		XRA(state->D);
		break;
	}
	case 0xb3:		//ORA E
	{
		XRA(state->E);
		break;
	}
	case 0xb4:		//ORA H
	{
		XRA(state->H);
		break;
	}
	case 0xb5:		//ORA L
	{
		XRA(state->L);
		break;
	}
	case 0xb6:		//ORA M
	{
		XRA(state->memory[state->HL_pair]);
		break;
	}
	case 0xb7:		//ORA A
	{
		XRA(state->A);
		break;
	}
	case 0xb8:		//CMP B
	{
		uint16_t result = (uint16_t)state->A - (uint16_t)state->B;
		(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
		break;
	}
	case 0xb9:		//CMP C
	{
		uint16_t result = (uint16_t)state->A - (uint16_t)state->C;
		(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
		break;
	}
	case 0xba:		//CMP D
	{
		uint16_t result = (uint16_t)state->A - (uint16_t)state->D;
		(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
		break;
	}
	case 0xbb:		//CMP E
	{
		uint16_t result = (uint16_t)state->A - (uint16_t)state->E;
		(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
		break;
	}
	case 0xbc:		//CMP H
	{
		uint16_t result = (uint16_t)state->A - (uint16_t)state->H;
		(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
		break;
	}
	case 0xbd:		//CMP L
	{
		uint16_t result = (uint16_t)state->A - (uint16_t)state->L;
		(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
		break;
	}
	case 0xbe:		//CMP M
	{
		uint16_t result = (uint16_t)state->A - (uint16_t)state->memory[state->HL_pair];
		(state->A == state->memory[state->HL_pair]) ? state->flag.Z = true : state->flag.Z = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		(state->A < state->memory[state->HL_pair]) ? state->flag.C = true : state->flag.C = false;
		parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
		break;
	}
	case 0xbf:		//CMP A
	{
		uint16_t result = (uint16_t)state->A - (uint16_t)state->A;
		(result & 0xff) == 0 ? state->flag.Z = true : state->flag.Z = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		parity(result & 0xff) ? state->flag.P = true : state->flag.P = false;
		break;
	}
	case 0xc0:		//RNZ
	{
		if (!state->flag.Z) {
			state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
			state->SP += 2;
		}
		break;
	}
	case 0xc1:		//POP B
	{
		state->C = state->memory[state->SP];
		state->B = state->memory[state->SP + 1];
		state->SP += 2;
	}
		break;
	case 0xc2:		//JNZ
	{
		if (!state->flag.Z) {
			uint16_t result = opcode[1] | (opcode[2] << 8);
			state->PC = result - 1;
		}
		else {
			state->PC += 2;
		}
		break;
	}
	case 0xc3:		//JMP
	{
		//compute the address
		uint16_t result = opcode[1] | (opcode[2] << 8);
		state->PC = result - 1;
		break;
	}
	case 0xc4:		//CNZ addr
	{
		if (!state->flag.Z)
		{
			uint16_t next = state->PC + 2;
			state->memory[state->SP - 1] = (next >> 8) & 0xff;
			state->memory[state->SP - 2] = next & 0xff;
			state->SP -= 2;
			state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
			state->PC -= 1;
		}
		else
		{
			state->PC += 2;
		}
		break;
	}
	case 0xc5:		//PUSH B
	{
		state->memory[state->SP - 1] = state->B;
		state->memory[state->SP - 2] = state->C;
		state->SP -= 2;
	}
	break;
	case 0xc6:		//ADI byte
	{
		ADD(opcode[1]);
		state->PC += 1; //advance one extra byte
		break;
	}
	case 0xc7:		//RST 0
		state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
		state->memory[state->SP - 2] = state->PC & 0xff;
		state->SP -= 2;
		state->PC = (8 * 0) & 0xff;
		state->PC -= 1;
		break;
	case 0xc8:		//RZ
	{
		if (state->flag.Z) {
			state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
			state->SP += 2;
		}
		break;
	}
	case 0xc9:		//RET
		state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
		state->SP += 2;
		break;
	case 0xca:		//JZ
	{
		if (state->flag.Z) {
			uint16_t result = opcode[1] | (opcode[2] << 8);
			state->PC = result - 1;
		}
		else {
			state->PC += 2;
		}
		break;
	}
	case 0xcb: std::cout << "0xcb -" << std::endl; break;
	case 0xcc:		//CZ addr
	{
		if (state->flag.Z)
		{
			uint16_t next = state->PC + 2;
			state->memory[state->SP - 1] = (next >> 8) & 0xff;
			state->memory[state->SP - 2] = next & 0xff;
			state->SP -= 2;
			state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
			state->PC -= 1;
		}
		else
		{
			state->PC += 2;
		}
		break;
	}
	case 0xcd:		//CALL
	{
		//if (5 == ((opcode[2] << 8) | opcode[1]))
		//{
		//	if (state->C == 9)
		//	{
		//		uint16_t offset = (state->D << 8) | (state->E);
		//		unsigned char* str = &state->memory[offset];  //skip the prefix bytes    
		//		while (*str != '$')
		//		{
		//			printf("%c", *str++);
		//		}
		//		printf("\n");
		//		state->PC += 2;
		//		break;
		//	}
		//	else if (state->C == 2)
		//	{
		//		//saw this in the inspected code, never saw it called    
		//		printf("print char routine called\n");
		//	}
		//	state->PC += 2;
		//	break;
		//}
		//else if (0 == ((opcode[2] << 8) | opcode[1]))
		//{
		//	exit(0);
		//	printf("smth happened here\n");
		//	state->PC += 2;
		//	break;
		//}
		//else
		uint16_t next = state->PC + 2;
		state->memory[state->SP - 1] = (next >> 8) & 0xff;
		state->memory[state->SP - 2] = (next & 0xff);
		state->SP -= 2;
		state->PC = (opcode[2] << 8) | (opcode[1]);
		state->PC -= 1;
		break;
	}
	case 0xce:		//ACI byte
	{
		ADC(opcode[1]);
		state->PC += 1; //advance one extra byte
		break;
	}
	case 0xcf:		//RST 1
		state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
		state->memory[state->SP - 2] = state->PC & 0xff;
		state->SP -= 2;
		state->PC = (8 * 1) & 0xff;
		state->PC -= 1;
		break;
	case 0xd0:		//RNC
	{
		if (!state->flag.C) {
			state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
			state->SP += 2;
		}
		break;
	}
	case 0xd1:		//POP D
	{
		state->E = state->memory[state->SP];
		state->D = state->memory[state->SP + 1];
		state->SP += 2;
	}
	break;
	case 0xd2:		//JNC
	{
		if (!state->flag.C) {
			uint16_t result = opcode[1] | (opcode[2] << 8);
			state->PC = result - 1;
		}
		else {
			state->PC += 2;
		}
		break;
	}
	case 0xd3:		//OUT D8
	{
		uint8_t port = opcode[1];
		uint16_t value = state->A;
		mOUT(port, value);
		state->PC += 1;
		break;
	}
	case 0xd4:		//CNC addr
	{
		if (!state->flag.C)
		{
			uint16_t next = state->PC + 2;
			state->memory[state->SP - 1] = (next >> 8) & 0xff;
			state->memory[state->SP - 2] = next & 0xff;
			state->SP -= 2;
			state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
			state->PC -= 1;
		}
		else
		{
			state->PC += 2;
		}
		break;
	}
	case 0xd5:		//PUSH D
	{
		state->memory[state->SP - 1] = state->D;
		state->memory[state->SP - 2] = state->E;
		state->SP -= 2;
	}
	break;
	case 0xd6:		//SUI D8
	{
		SUB(opcode[1]);
		state->PC += 1;
		break;
	}
	case 0xd7:		//RST 2
		state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
		state->memory[state->SP - 2] = state->PC & 0xff;
		state->SP -= 2;
		state->PC = (8 * 2) & 0xff;
		state->PC -= 1;
		break;
	case 0xd8:		//RC
	{
		if (state->flag.C) {
			state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
			state->SP += 2;
		}
		break;
	}
	case 0xd9: std::cout << "0xd9 -" << std::endl; break;
	case 0xda:		//JC
	{
		if (state->flag.C) {
			uint16_t result = opcode[1] | (opcode[2] << 8);
			state->PC = result - 1;
		}
		else {
			state->PC += 2;
		}
		break;
	}
	case 0xdb:		//IN
	{
		mIN(opcode[1]);
		state->PC += 1;
	}
		break;
	case 0xdc:		//CC addr
	{
		if (state->flag.C)
		{
			uint16_t next = state->PC + 2;
			state->memory[state->SP - 1] = (next >> 8) & 0xff;
			state->memory[state->SP - 2] = next & 0xff;
			state->SP -= 2;
			state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
			state->PC -= 1;
		}
		else
		{
			state->PC += 2;
		}
		break;
	}
	case 0xdd: std::cout << "0xdd -" << std::endl; break;
	case 0xde:		//SBI byte
	{
		SBB(opcode[1]);
		state->PC += 1; //advance one extra byte
		break;
	}
	case 0xdf:		//RST 3
		state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
		state->memory[state->SP - 2] = state->PC & 0xff;
		state->SP -= 2;
		state->PC = (8 * 3) & 0xff;
		state->PC -= 1;
		break;
	case 0xe0:		//RPO
	{
		if (!state->flag.P) {
			state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
			state->SP += 2;
		}
		break;
	}
	case 0xe1:		//POP H
	{
		state->L = state->memory[state->SP];
		state->H = state->memory[state->SP + 1];
		state->SP += 2;
	}
	break;
	case 0xe2:		//JPO
	{
		if (!state->flag.P) {
			uint16_t result = opcode[1] | (opcode[2] << 8);
			state->PC = result - 1;
		}
		else {
			state->PC += 2;
		}
		break;
	}
	case 0xe3:		//XTHL
	{
		std::swap(state->L, state->memory[state->SP]);
		std::swap(state->H, state->memory[state->SP + 1]);
		break;
	}
	case 0xe4:		//CPO addr
	{
		if (!state->flag.P)
		{
			uint16_t next = state->PC + 2;
			state->memory[state->SP - 1] = (next >> 8) & 0xff;
			state->memory[state->SP - 2] = next & 0xff;
			state->SP -= 2;
			state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
			state->PC -= 1;
		}
		else
		{
			state->PC += 2;
		}
		break;
	}
	case 0xe5:		//PUSH H
	{
		state->memory[state->SP - 1] = state->H;
		state->memory[state->SP - 2] = state->L;
		state->SP -= 2;
	}
	break;
	case 0xe6:		//ANI D8
	{
		ANA(opcode[1]);
		state->PC += 1;
		break;
	}
	case 0xe7:		//RST 4
		state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
		state->memory[state->SP - 2] = state->PC & 0xff;
		state->SP -= 2;
		state->PC = (8 * 4) & 0xff;
		state->PC -= 1;
		break;
	case 0xe8:		//RPE
	{
		if (state->flag.P) {
			state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
			state->SP += 2;
		}
		break;
	}
	case 0xe9:		//PCHL
	{
		uint16_t address = (state->H << 8) | state->L;
		state->PC = address;
		state->PC -= 1;
		break;
	}
	case 0xea:		//JPE
	{
		if (state->flag.P) {
			uint16_t result = opcode[1] | (opcode[2] << 8);
			state->PC = result - 1;
		}
		else {
			state->PC += 2;
		}
		break;
	}
	case 0xeb:		//XCHG
	{
		XCHG();
		break;
	}
	case 0xec:		//CPE addr
	{
		if (state->flag.P)
		{
			uint16_t next = state->PC + 2;
			state->memory[state->SP - 1] = (next >> 8) & 0xff;
			state->memory[state->SP - 2] = next & 0xff;
			state->SP -= 2;
			state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
			state->PC -= 1;
		}
		else
		{
			state->PC += 2;
		}
		break;
	}
	case 0xed: std::cout << "0xed -" << std::endl; break;
	case 0xee:		//XRI B8
	{
		XRA(opcode[1]);
		state->PC++;
		break;
	}
	case 0xef:		//RST 5
		state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
		state->memory[state->SP - 2] = state->PC & 0xff;
		state->SP -= 2;
		state->PC = (8 * 5) & 0xff;
		state->PC -= 1;
		break;
	case 0xf0:		//RP
	{
		if (!state->flag.S) {
			state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
			state->SP += 2;
		}
		break;
	}
	case 0xf1:		//POP PSW
	{
		state->A = state->memory[state->SP + 1];
		uint8_t psw = state->memory[state->SP];
		state->flag.Z = (0x01 == (psw & 0x01));
		state->flag.S = (0x02 == (psw & 0x02));
		state->flag.P = (0x04 == (psw & 0x04));
		state->flag.C = (0x05 == (psw & 0x08));
		state->flag.AC = (0x10 == (psw & 0x10));
		state->SP += 2;
		break;
	}
	case 0xf2:		//JP
	{
		if (!state->flag.S) {
			uint16_t result = opcode[1] | (opcode[2] << 8);
			state->PC = result - 1;
		}
		else {
			state->PC += 2;
		}
		break;
	}
	case 0xf3: std::cout << "0xf3 DI special" << std::endl; break;
	case 0xf4:		//CP addr
	{
		if (!state->flag.S)
		{
			uint16_t next = state->PC + 2;
			state->memory[state->SP - 1] = (next >> 8) & 0xff;
			state->memory[state->SP - 2] = next & 0xff;
			state->SP -= 2;
			state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
			state->PC -= 1;
		}
		else
		{
			state->PC += 2;
		}
		break;
	}
	case 0xf5:		//PUSH PSW
	{
		state->memory[state->SP - 1] = state->A;
		uint8_t psw = (state->flag.Z |
			(state->flag.S << 1) |
			(state->flag.P << 2) |
			(state->flag.C << 3) |
			(state->flag.AC << 4));
		state->memory[state->SP - 2] = psw;
		state->SP = state->SP - 2;
		break;
	}
	case 0xf6:		//ORI B8
	{
		ORA(opcode[1]);
		state->PC++;
		break;
	}
	case 0xf7:		//RST 6
		state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
		state->memory[state->SP - 2] = state->PC & 0xff;
		state->SP -= 2;
		state->PC = (8 * 6) & 0xff;
		state->PC -= 1;
		break;
	case 0xf8:		//RM
	{
		if (state->flag.S) {
			state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
			state->SP += 2;
		}
		break;
	}
	case 0xf9:		//SPHL
		state->SP = state->HL_pair;
		break;
	case 0xfa:		//JM addr
	{
		if (state->flag.S)
		{
			state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
			state->PC -= 1;
		}
		else
		{
			state->PC += 2;
		}
		break;
	}
	case 0xfb:		//EI
	{
		state->isOn = true;
		break;
	}
	case 0xfc:		//CM addr
	{
		if (state->flag.S)
		{
			uint16_t next = state->PC + 2;
			state->memory[state->SP - 1] = (next >> 8) & 0xff;
			state->memory[state->SP - 2] = next & 0xff;
			state->SP -= 2;
			state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
			state->PC -= 1;
		}
		else
		{
			state->PC += 2;
		}
		break;
	}
	case 0xfd: std::cout << "0xfd -" << std::endl; break;
	case 0xfe:		//CPI D8
	{
		uint8_t result = state->A - opcode[1];
		state->A == opcode[1] ? state->flag.Z = true : state->flag.Z = false;
		parity(result) ? state->flag.P = true : state->flag.P = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		state->A < opcode[1] ? state->flag.C = true : state->flag.C = false;
		state->PC += 1;
		break;
	}
	case 0xff:		//RST 7
	{
		state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
		state->memory[state->SP - 2] = state->PC & 0xff;
		state->SP -= 2;
		state->PC = (8 * 7) & 0xff;
		state->PC -= 1;
		break;
	}
	}
	state->PC += 1;
	++state->count;
}
