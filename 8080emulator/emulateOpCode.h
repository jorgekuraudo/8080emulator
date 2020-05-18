#pragma once
#include <iostream>
#include "registers.h"

//data bus
struct bus
{
	uint8_t port1{ 0 };
	uint8_t port2{ 0 };
} bus;

//system interrupt
void interrupt(State8080* state, int num) {
	state->memory[state->SP - 1] = (state->PC >> 8) & 0xff;
	state->memory[state->SP - 2] = state->PC & 0xff;
	state->SP -= 2;
	state->PC = 8 * num;
}

//hardware shift register
struct shift_register
{
	uint16_t status = 0x0000;
	uint8_t result = 0x00;
} shift16;

//mIN and mOUT deal with system's I/O
uint8_t mIN(uint8_t port) {
	switch (port)
	{
	case 1:
	{
		return bus.port1;
		break;
	}
	case 2:
	{
		return bus.port2;
		break;
	}
	case 3:
	{
		return shift16.result;
		break;
	}
	}
}
void mOUT(uint8_t port, uint8_t value) {
	switch (port)
	{
	case 2:
	{
		int offset = value & 0x7;
		shift16.result = (shift16.status << offset) & 0xff00;
		break;
	}
	case 4:
	{
		shift16.status >> 8;
		shift16.status = (shift16.status & 0x00ff) | (value << 8);
		break;
	}
	}
}

void emulateOpCode(State8080* state) {
	uint8_t* opcode = &state->memory[state->PC];


	//statements for debugging
	state->count;
	uint16_t HL_pair = state->L | (state->H << 8);
	uint16_t BC_pair = state->C | (state->B << 8);
	uint16_t DE_pair = state->E | (state->D << 8);
	printf("COUNT: %d\tINST: %x\tOPCODE: %x\tSP: %x\tBC: %x\tDE: %x\tHL: %x\t\n", state->count, (int)state->PC, *opcode, state->SP, BC_pair, DE_pair, HL_pair);
	/*if (state->count % 1000 == 0) {
		printf("COUNT: %d\tINST: %x\tOPCODE: %x\tSP: %x\tBC: %x\tDE: %x\tHL: %x\t\n", state->count, (int)state->PC, *opcode, state->SP, BC_pair, DE_pair, HL_pair);
	}*/
	/*if (state->count > 50000) {
		state->memory[0x20c0] = 0;
		printf("COUNT: %d\tINST: %x\tOPCODE: %x\tSP: %x\tBC: %x\tDE: %x\tHL: %x\t\n", state->count, (int)state->PC, *opcode, state->SP, BC_pair, DE_pair, HL_pair);

	}*/

	switch (*opcode)
	{
	case 0x0: break; //NOP
	case 0x1:		//LXI B,D16
		state->B = opcode[2];
		state->C = opcode[1];
		state->PC += 2;
		break;
	case 0x2: std::cout << "Not implemented yet" << std::endl; break;
	case 0x3:		//INX B
	{
		uint32_t BC_pair = state->C | (state->B << 8);
		++BC_pair;
		state->C = BC_pair & 0xff;
		state->B = (BC_pair & 0xff00) >> 8;
		break;
	}
	case 0x4: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5:		//DCR B
	{
		state->B -= 0x01;
		state->B == 0 ? state->flag.Z = true : state->flag.Z = false;
		state->B % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		state->B & 0x80 != 0 ? state->flag.S = true : state->flag.S = false;
		break;
	}
	case 0x6:		//MVI B,D8
		state->B = opcode[1];
		state->PC += 1;
		break;
	case 0x7: std::cout << "Not implemented yet" << std::endl; break;
	case 0x8: std::cout << "Not implemented yet" << std::endl; break;
	case 0x9:		//DAD B
	{
		uint32_t HL_pair = state->L | (state->H << 8);
		uint32_t BC_pair = state->C | (state->B << 8);
		uint32_t res = HL_pair + BC_pair;
		
		//set carry flag:
		state->flag.C = (res & 0xffff0000) != 0;
		//set H and L registers
		state->L = res & 0xff;
		state->H = (res & 0xff00) >> 8;
		break;
	}
	case 0xa: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb: std::cout << "Not implemented yet" << std::endl; break;
	case 0xc: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd:		//DCR C
	{
		state->C -= 1;
		state->C == 0 ? state->flag.Z = true : state->flag.Z = false;
		state->C % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		state->C & 0x80 != 0 ? state->flag.S = true : state->flag.S = false;
		break;
	}
	case 0xe:		//MVI C,D8
		state->C = opcode[1];
		state->PC += 1;
		break;
	case 0xf:		//RRC
	{
		uint8_t x = state->A;
		state->A = ((x & 1) << 7) | (x >> 1);
		state->flag.C = (1 == (x & 1));
		break;
	}
	case 0x10: std::cout << "Not implemented yet" << std::endl; break;
	case 0x11:		//LXI D, D16
		state->D = opcode[2];
		state->E = opcode[1];
		state->PC += 2;
		break;
	case 0x12: std::cout << "Not implemented yet" << std::endl; break;
	case 0x13:		//INX D
	{
		uint32_t DE_pair = state->E | (state->D << 8);
		++DE_pair;
		state->E = DE_pair & 0xff;
		state->D = (DE_pair & 0xff00) >> 8;
		break;
	}
	case 0x14: std::cout << "Not implemented yet" << std::endl; break;
	case 0x15: std::cout << "Not implemented yet" << std::endl; break;
	case 0x16: std::cout << "Not implemented yet" << std::endl; break;
	case 0x17: std::cout << "Not implemented yet" << std::endl; break;
	case 0x18: std::cout << "Not implemented yet" << std::endl; break;
	case 0x19:		//DAD D
	{
		uint32_t HL_pair = state->L | (state->H << 8);
		uint32_t DE_pair = state->E | (state->D << 8);
		uint32_t res = HL_pair + DE_pair;

		//set carry flag:
		state->flag.C = (res & 0xffff0000) != 0;
		//set H and L registers
		state->L = res & 0xff;
		state->H = (res & 0xff00) >> 8;
		break;
	}
	case 0x1a:		//LDAX D
	{
		uint16_t address = (state->D << 8) | state->E;
		state->A = state->memory[address];
		break;
	}
	case 0x1b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x20: std::cout << "Not implemented yet" << std::endl; break;
	case 0x21:		//LXI H, D16
		state->H = opcode[2];
		state->L = opcode[1];
		state->PC += 2;
		break;
	case 0x22: std::cout << "Not implemented yet" << std::endl; break;
	case 0x23:		//INX H
	{
		uint32_t HL_pair = state->L | (state->H << 8);
		++HL_pair;
		state->L = HL_pair & 0xff;
		state->H = (HL_pair & 0xff00) >> 8;
		break;
	}
	case 0x24: std::cout << "Not implemented yet" << std::endl; break;
	case 0x25: std::cout << "Not implemented yet" << std::endl; break;
	case 0x26:		//MVI H,D8
		state->H = opcode[1];
		state->PC += 1;
		break;
	case 0x27: std::cout << "Not implemented yet" << std::endl; break;
	case 0x28: std::cout << "Not implemented yet" << std::endl; break;
	case 0x29:		//DAD H
	{
		uint32_t HL_pair = (state->H << 8) | state->L;
		uint32_t res = HL_pair + HL_pair;
		state->H = (res & 0xff00) >> 8;
		state->L = res & 0xff;
		state->flag.C = ((res & 0xffff0000) != 0);
	}
	break;
	case 0x2a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2f:		//CMA
		state->A = ~state->A;
		break;
	case 0x30: std::cout << "Not implemented yet" << std::endl; break;
	case 0x31:		//LXI SP, D16
		state->SP = opcode[1] | (opcode[2] << 8);
		state->PC += 2;
		break;
	case 0x32:		//STA
	{
		uint16_t address = (opcode[2] << 8) | opcode[1];
		state->memory[address] = state->A;
		state->PC += 2;
		break;
	}
	case 0x33: std::cout << "Not implemented yet" << std::endl; break;
	case 0x34: std::cout << "Not implemented yet" << std::endl; break;
	case 0x35:		//DCR M
	{
		uint16_t HL_pair = state->L | (state->H << 8);
		uint8_t res = state->memory[HL_pair] -= 1;
		res == 0 ? state->flag.Z = true : state->flag.Z = false;
		res % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		res & 0x80 != 0 ? state->flag.S = true : state->flag.S = false;
		break;

	}
	case 0x36:		//MVI M,D8
	{
		uint16_t address = (state->H << 8) | state->L;
		state->memory[address] = opcode[1];
		state->PC += 1;
		break;
	}
	case 0x37: std::cout << "Not implemented yet" << std::endl; break;
	case 0x38: std::cout << "Not implemented yet" << std::endl; break;
	case 0x39: std::cout << "Not implemented yet" << std::endl; break;
	case 0x3a:		//LDA addr
	{
		uint16_t address = (opcode[2] << 8) | opcode[1];
		state->A = state->memory[address];
		state->PC += 2;
		break;
	}
	case 0x3b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x3c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x3d: //DCR A
	{
		state->A -= 0x01;
		state->A == 0 ? state->flag.Z = true : state->flag.Z = false;
		state->A % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		state->A & 0x80 != 0 ? state->flag.S = true : state->flag.S = false;
		break;
	}
	case 0x3e:		//MVI A,D8
		state->A = opcode[1];
		state->PC += 1;
		break;
	case 0x3f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x40: std::cout << "Not implemented yet" << std::endl; break;
	case 0x41: std::cout << "Not implemented yet" << std::endl; break;
	case 0x42: std::cout << "Not implemented yet" << std::endl; break;
	case 0x43: std::cout << "Not implemented yet" << std::endl; break;
	case 0x44: std::cout << "Not implemented yet" << std::endl; break;
	case 0x45: std::cout << "Not implemented yet" << std::endl; break;
	case 0x46: std::cout << "Not implemented yet" << std::endl; break;
	case 0x47: std::cout << "Not implemented yet" << std::endl; break;
	case 0x48: std::cout << "Not implemented yet" << std::endl; break;
	case 0x49: std::cout << "Not implemented yet" << std::endl; break;
	case 0x4a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x4b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x4c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x4d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x4e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x4f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x50: std::cout << "Not implemented yet" << std::endl; break;
	case 0x51: std::cout << "Not implemented yet" << std::endl; break;
	case 0x52: std::cout << "Not implemented yet" << std::endl; break;
	case 0x53: std::cout << "Not implemented yet" << std::endl; break;
	case 0x54: std::cout << "Not implemented yet" << std::endl; break;
	case 0x55: std::cout << "Not implemented yet" << std::endl; break;
	case 0x56:		//MOV D,M
	{
		uint16_t address = (state->H << 8) | state->L;
		state->D = state->memory[address];
		break;
	}
	case 0x57: std::cout << "Not implemented yet" << std::endl; break;
	case 0x58: std::cout << "Not implemented yet" << std::endl; break;
	case 0x59: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5e:		//MOV E,M
	{
		uint16_t address = (state->H << 8) | state->L;
		state->E = state->memory[address];
		break;
	}
	case 0x5f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x60: std::cout << "Not implemented yet" << std::endl; break;
	case 0x61: std::cout << "Not implemented yet" << std::endl; break;
	case 0x62: std::cout << "Not implemented yet" << std::endl; break;
	case 0x63: std::cout << "Not implemented yet" << std::endl; break;
	case 0x64: std::cout << "Not implemented yet" << std::endl; break;
	case 0x65: std::cout << "Not implemented yet" << std::endl; break;
	case 0x66:		//MOV H,M
	{
		uint16_t address = (state->H << 8) | state->L;
		state->H = state->memory[address];
		break;
	}
	case 0x67: std::cout << "Not implemented yet" << std::endl; break;
	case 0x68: std::cout << "Not implemented yet" << std::endl; break;
	case 0x69: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6f:		//MOV L,A
	{
		state->L = state->A;
		break;
	}
	case 0x70: std::cout << "Not implemented yet" << std::endl; break;
	case 0x71: std::cout << "Not implemented yet" << std::endl; break;
	case 0x72: std::cout << "Not implemented yet" << std::endl; break;
	case 0x73: std::cout << "Not implemented yet" << std::endl; break;
	case 0x74: std::cout << "Not implemented yet" << std::endl; break;
	case 0x75: std::cout << "Not implemented yet" << std::endl; break;
	case 0x76: std::cout << "Not implemented yet" << std::endl; break;
	case 0x77:		//MOV M,A
	{
		uint16_t address = (state->H << 8) | state->L;
		state->memory[address] = state->A;
		break;
	}
	case 0x78: std::cout << "Not implemented yet" << std::endl; break;
	case 0x79: std::cout << "Not implemented yet" << std::endl; break;
	case 0x7a:		//MOV A,D
	{
		state->A = state->D;
		break;
	}
	case 0x7b:		//MOV A,E
	{
		state->A = state->E;
		break;
	}
	case 0x7c:		//MOV A,H
	{
		state->A = state->H;
		break;
	}
	case 0x7d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x7e:		//MOV A,M
	{
		uint16_t address = (state->H << 8) | state->L;
		state->A = state->memory[address];
		break;
	}
	case 0x7f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x80:		//ADD B
	{
		uint16_t result = (uint16_t)state->A + (uint16_t)state->B;
		//flags:
		//Zero flag
		result & 0xff == 0 ? state->flag.Z = true : state->flag.Z = false;
		//Sign flag
		result & 0x80 != 0 ? state->flag.S = true : state->flag.S = false;
		//Carry flag
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		//Parity flag
		(result & 0xff) % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		//Auxiliary carry flag
		//don't implement yet

		state->A = result & 0xff; //add result to accumulator (only the 8 low bits)

		break;
	}
	case 0x81: std::cout << "Not implemented yet" << std::endl; break;
	case 0x82: std::cout << "Not implemented yet" << std::endl; break;
	case 0x83: std::cout << "Not implemented yet" << std::endl; break;
	case 0x84: std::cout << "Not implemented yet" << std::endl; break;
	case 0x85: std::cout << "Not implemented yet" << std::endl; break;
	case 0x86: std::cout << "Not implemented yet" << std::endl; break;
	case 0x87: std::cout << "Not implemented yet" << std::endl; break;
	case 0x88:		//ADC B
	{
		uint16_t result = (uint16_t)state->A + (uint16_t)state->B + 0x01; //added bit from carry
		//flags:
		//Zero flag
		result & 0xff == 0 ? state->flag.Z = true : state->flag.Z = false;
		//Sign flag
		result & 0x80 != 0 ? state->flag.S = true : state->flag.S = false;
		//Carry flag
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		//Parity flag
		(result & 0xff) % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		//Auxiliary carry flag
		//don't implement yet

		state->A = result & 0xff; //add result to accumulator (only the 8 low bits)
		break;
	}
	case 0x89: std::cout << "Not implemented yet" << std::endl; break;
	case 0x8a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x8b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x8c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x8d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x8e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x8f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x90: std::cout << "Not implemented yet" << std::endl; break;
	case 0x91: std::cout << "Not implemented yet" << std::endl; break;
	case 0x92: std::cout << "Not implemented yet" << std::endl; break;
	case 0x93: std::cout << "Not implemented yet" << std::endl; break;
	case 0x94: std::cout << "Not implemented yet" << std::endl; break;
	case 0x95: std::cout << "Not implemented yet" << std::endl; break;
	case 0x96: std::cout << "Not implemented yet" << std::endl; break;
	case 0x97: std::cout << "Not implemented yet" << std::endl; break;
	case 0x98: std::cout << "Not implemented yet" << std::endl; break;
	case 0x99: std::cout << "Not implemented yet" << std::endl; break;
	case 0x9a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x9b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x9c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x9d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x9e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x9f: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa0: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa1: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa2: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa3: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa5: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa6: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa7:		//ANA A
	{
		state->A = state->A & state->A;
		state->A == 0 ? state->flag.Z = true : state->flag.Z = false;
		state->A % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		(state->A & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		state->flag.C = false;
		break;
	}
	case 0xa8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa9: std::cout << "Not implemented yet" << std::endl; break;
	case 0xaa: std::cout << "Not implemented yet" << std::endl; break;
	case 0xab: std::cout << "Not implemented yet" << std::endl; break;
	case 0xac: std::cout << "Not implemented yet" << std::endl; break;
	case 0xad: std::cout << "Not implemented yet" << std::endl; break;
	case 0xae: std::cout << "Not implemented yet" << std::endl; break;
	case 0xaf:		//XRA A
	{
		state->A = state->A | state->A;
		state->A == 0 ? state->flag.Z = true : state->flag.Z = false;
		state->A % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		(state->A & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		state->flag.C = false;
		state->flag.AC = false;
		break;
	}
	case 0xb0: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb1: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb2: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb3: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb5: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb6: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb7: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb9: std::cout << "Not implemented yet" << std::endl; break;
	case 0xba: std::cout << "Not implemented yet" << std::endl; break;
	case 0xbb: std::cout << "Not implemented yet" << std::endl; break;
	case 0xbc: std::cout << "Not implemented yet" << std::endl; break;
	case 0xbd: std::cout << "Not implemented yet" << std::endl; break;
	case 0xbe: std::cout << "Not implemented yet" << std::endl; break;
	case 0xbf: std::cout << "Not implemented yet" << std::endl; break;
	case 0xc0: std::cout << "Not implemented yet" << std::endl; break;
	case 0xc1:		//POP B
	{
		state->C = state->memory[state->SP];
		state->B = state->memory[state->SP + 1];
		state->SP += 2;
	}
		break;
	case 0xc2:		//JMZ
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
	case 0xc4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xc5:		//PUSH B
	{
		state->memory[state->SP - 1] = state->B;
		state->memory[state->SP - 2] = state->C;
		state->SP -= 2;
	}
	break;
	case 0xc6:		//ADI byte
	{
		uint16_t result = (uint16_t)state->A + (uint16_t)opcode[1];
		//flags:
		//Zero flag
		result & 0xff == 0 ? state->flag.Z = true : state->flag.Z = false;
		//Sign flag
		result & 0x80 != 0 ? state->flag.S = true : state->flag.S = false;
		//Carry flag
		result > 0xff ? state->flag.C = true : state->flag.C = false;
		//Parity flag
		(result & 0xff) % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		//Auxiliary carry flag
		//don't implement yet

		state->A = result & 0xff; //add result to accumulator (only the 8 low bits)
		state->PC += 1; //advance one extra byte
		break;
	}
	case 0xc7: std::cout << "Not implemented yet" << std::endl; break;
	case 0xc8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xc9:		//RET
		state->PC = state->memory[state->SP] | (state->memory[state->SP + 1] << 8);
		//state->PC -= 1;
		state->SP += 2;
		break;
	case 0xca: std::cout << "Not implemented yet" << std::endl; break;
	case 0xcb: std::cout << "Not implemented yet" << std::endl; break;
	case 0xcc: std::cout << "Not implemented yet" << std::endl; break;
	case 0xcd:		//CALL
	{
		uint16_t next = state->PC + 2;
		state->memory[state->SP - 1] = (next >> 8) & 0xff;
		state->memory[state->SP - 2] = next & 0xff;
		state->SP -= 2;
		state->PC = (opcode[2] << 8) | (opcode[1] & 0xff);
		state->PC -= 1;
		break;
	}
	case 0xce: std::cout << "Not implemented yet" << std::endl; break;
	case 0xcf: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd0: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd1:		//POP D
	{
		state->E = state->memory[state->SP];
		state->D = state->memory[state->SP + 1];
		state->SP += 2;
	}
	break;
	case 0xd2: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd3:		//OUT D8
	{
		uint8_t port = opcode[1];
		uint8_t value = state->A;
		mOUT(port, value);
		state->PC += 1;
		break;
	}
	case 0xd4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd5:		//PUSH D
	{
		state->memory[state->SP - 1] = state->D;
		state->memory[state->SP - 2] = state->E;
		state->SP -= 2;
	}
	break;
	case 0xd6: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd7: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd9: std::cout << "Not implemented yet" << std::endl; break;
	case 0xda: std::cout << "Not implemented yet" << std::endl; break;
	case 0xdb:		//IN
	{
		uint8_t port = opcode[1];
		state->A = mIN(port);
		state->PC += 1;
	}
		break;
	case 0xdc: std::cout << "Not implemented yet" << std::endl; break;
	case 0xdd: std::cout << "Not implemented yet" << std::endl; break;
	case 0xde: std::cout << "Not implemented yet" << std::endl; break;
	case 0xdf: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe0: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe1:		//POP H
	{
		state->L = state->memory[state->SP];
		state->H = state->memory[state->SP + 1];
		state->SP += 2;
	}
	break;
	case 0xe2: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe3: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe5:		//PUSH H
	{
		state->memory[state->SP - 1] = state->H;
		state->memory[state->SP - 2] = state->L;
		state->SP -= 2;
	}
	break;
	case 0xe6:		//ANI D8
	{
		uint8_t res = opcode[1] & state->A;
		//clear AC and C flags
		state->flag.AC = false;
		state->flag.C = false;
		res == 0 ? state->flag.Z = true : state->flag.Z = false;
		res % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		(res & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		state->PC += 1;
		break;
	}
	case 0xe7: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe9: std::cout << "Not implemented yet" << std::endl; break;
	case 0xea: std::cout << "Not implemented yet" << std::endl; break;
	case 0xeb:		//XCHG
	{
		std::swap(state->H, state->D);
		std::swap(state->L, state->E);
		break;
	}
	case 0xec: std::cout << "Not implemented yet" << std::endl; break;
	case 0xed: std::cout << "Not implemented yet" << std::endl; break;
	case 0xee: std::cout << "Not implemented yet" << std::endl; break;
	case 0xef: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf0: std::cout << "Not implemented yet" << std::endl; break;
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
	case 0xf2: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf3: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf5:		//PUSH PSW
	{
		state->memory[state->SP - 1] = state->A;
		uint8_t psw = (state->flag.Z |
			state->flag.S << 1 |
			state->flag.P << 2 |
			state->flag.C << 3 |
			state->flag.AC << 4);
		state->memory[state->SP - 2] = psw;
		state->SP = state->SP - 2;
		break;
	}
	case 0xf6: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf7: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf9: std::cout << "Not implemented yet" << std::endl; break;
	case 0xfa: std::cout << "Not implemented yet" << std::endl; break;
	case 0xfb:		//EI
	{
		state->isOn = true;
		break;
	}
	case 0xfc: std::cout << "Not implemented yet" << std::endl; break;
	case 0xfd: std::cout << "Not implemented yet" << std::endl; break;
	case 0xfe:		//CPI D8
	{
		uint8_t result = state->A - opcode[1];
		state->A == opcode[1] ? state->flag.Z = true : state->flag.Z = false;
		result % 2 == 0 ? state->flag.P = true : state->flag.P = false;
		(result & 0x80) != 0 ? state->flag.S = true : state->flag.S = false;
		state->A < opcode[1] ? state->flag.C = true : state->flag.C = false;
		state->PC += 1;
		break;
	}
	case 0xff: std::cout << "Not implemented yet" << std::endl; break;
	}
	state->PC += 1;
	++state->count;
}
