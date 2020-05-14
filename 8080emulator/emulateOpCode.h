#pragma once
#include <iostream>
#include "registers.h"

void emulateOpCode(State8080* state) {
	uint8_t* opcode = &state->memory[state->PC];

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
		uint16_t BC_pair = state->C | (state->B << 8);
		uint16_t result = BC_pair + 0x01;
		state->C = BC_pair & 0xff;
		state->B = (BC_pair >> 8) & 0xff;
		break;
	}
	case 0x4: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6: std::cout << "Not implemented yet" << std::endl; break;
	case 0x7: std::cout << "Not implemented yet" << std::endl; break;
	case 0x8: std::cout << "Not implemented yet" << std::endl; break;
	case 0x9:		//DAD B
	{
		uint16_t HL_pair = state->L | (state->H << 8);
		uint16_t BC_pair = state->C | (state->B << 8);
		HL_pair += BC_pair;
		//set carry flag:
		HL_pair & 0x80 != 0 ? state->flag.C = true : state->flag.C = false;
		//set H and L registers
		state->L = HL_pair & 0xff;
		state->H = (HL_pair >> 8) & 0xff;
		break;
	}
	case 0xa: std::cout << "Not implemented yet" << std::endl; break;
	case 0xb: std::cout << "Not implemented yet" << std::endl; break;
	case 0xc: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf: std::cout << "Not implemented yet" << std::endl; break;
	case 0x10: std::cout << "Not implemented yet" << std::endl; break;
	case 0x11: std::cout << "Not implemented yet" << std::endl; break;
	case 0x12: std::cout << "Not implemented yet" << std::endl; break;
	case 0x13: std::cout << "Not implemented yet" << std::endl; break;
	case 0x14: std::cout << "Not implemented yet" << std::endl; break;
	case 0x15: std::cout << "Not implemented yet" << std::endl; break;
	case 0x16: std::cout << "Not implemented yet" << std::endl; break;
	case 0x17: std::cout << "Not implemented yet" << std::endl; break;
	case 0x18: std::cout << "Not implemented yet" << std::endl; break;
	case 0x19: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x1f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x20: std::cout << "Not implemented yet" << std::endl; break;
	case 0x21: std::cout << "Not implemented yet" << std::endl; break;
	case 0x22: std::cout << "Not implemented yet" << std::endl; break;
	case 0x23: std::cout << "Not implemented yet" << std::endl; break;
	case 0x24: std::cout << "Not implemented yet" << std::endl; break;
	case 0x25: std::cout << "Not implemented yet" << std::endl; break;
	case 0x26: std::cout << "Not implemented yet" << std::endl; break;
	case 0x27: std::cout << "Not implemented yet" << std::endl; break;
	case 0x28: std::cout << "Not implemented yet" << std::endl; break;
	case 0x29: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x2f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x30: std::cout << "Not implemented yet" << std::endl; break;
	case 0x31: std::cout << "Not implemented yet" << std::endl; break;
	case 0x32: std::cout << "Not implemented yet" << std::endl; break;
	case 0x33: std::cout << "Not implemented yet" << std::endl; break;
	case 0x34: std::cout << "Not implemented yet" << std::endl; break;
	case 0x35: std::cout << "Not implemented yet" << std::endl; break;
	case 0x36: std::cout << "Not implemented yet" << std::endl; break;
	case 0x37: std::cout << "Not implemented yet" << std::endl; break;
	case 0x38: std::cout << "Not implemented yet" << std::endl; break;
	case 0x39: std::cout << "Not implemented yet" << std::endl; break;
	case 0x3a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x3b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x3c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x3d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x3e: std::cout << "Not implemented yet" << std::endl; break;
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
	case 0x56: std::cout << "Not implemented yet" << std::endl; break;
	case 0x57: std::cout << "Not implemented yet" << std::endl; break;
	case 0x58: std::cout << "Not implemented yet" << std::endl; break;
	case 0x59: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x5f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x60: std::cout << "Not implemented yet" << std::endl; break;
	case 0x61: std::cout << "Not implemented yet" << std::endl; break;
	case 0x62: std::cout << "Not implemented yet" << std::endl; break;
	case 0x63: std::cout << "Not implemented yet" << std::endl; break;
	case 0x64: std::cout << "Not implemented yet" << std::endl; break;
	case 0x65: std::cout << "Not implemented yet" << std::endl; break;
	case 0x66: std::cout << "Not implemented yet" << std::endl; break;
	case 0x67: std::cout << "Not implemented yet" << std::endl; break;
	case 0x68: std::cout << "Not implemented yet" << std::endl; break;
	case 0x69: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6e: std::cout << "Not implemented yet" << std::endl; break;
	case 0x6f: std::cout << "Not implemented yet" << std::endl; break;
	case 0x70: std::cout << "Not implemented yet" << std::endl; break;
	case 0x71: std::cout << "Not implemented yet" << std::endl; break;
	case 0x72: std::cout << "Not implemented yet" << std::endl; break;
	case 0x73: std::cout << "Not implemented yet" << std::endl; break;
	case 0x74: std::cout << "Not implemented yet" << std::endl; break;
	case 0x75: std::cout << "Not implemented yet" << std::endl; break;
	case 0x76: std::cout << "Not implemented yet" << std::endl; break;
	case 0x77: std::cout << "Not implemented yet" << std::endl; break;
	case 0x78: std::cout << "Not implemented yet" << std::endl; break;
	case 0x79: std::cout << "Not implemented yet" << std::endl; break;
	case 0x7a: std::cout << "Not implemented yet" << std::endl; break;
	case 0x7b: std::cout << "Not implemented yet" << std::endl; break;
	case 0x7c: std::cout << "Not implemented yet" << std::endl; break;
	case 0x7d: std::cout << "Not implemented yet" << std::endl; break;
	case 0x7e: std::cout << "Not implemented yet" << std::endl; break;
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
	case 0xa7: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xa9: std::cout << "Not implemented yet" << std::endl; break;
	case 0xaa: std::cout << "Not implemented yet" << std::endl; break;
	case 0xab: std::cout << "Not implemented yet" << std::endl; break;
	case 0xac: std::cout << "Not implemented yet" << std::endl; break;
	case 0xad: std::cout << "Not implemented yet" << std::endl; break;
	case 0xae: std::cout << "Not implemented yet" << std::endl; break;
	case 0xaf: std::cout << "Not implemented yet" << std::endl; break;
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
	case 0xc1: std::cout << "Not implemented yet" << std::endl; break;
	case 0xc2:		//JMZ
	{
		if (state->flag.Z) {
			uint16_t result = opcode[2] | (opcode[1] << 8);
			state->PC = result;
		}
		else {
			state->PC += 2;
		}
		break;
	}
	case 0xc3:		//JMP
	{
		//compute the address
		uint16_t result = opcode[2] | (opcode[1] << 8);
		state->PC = result;
		break;
	}
	case 0xc4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xc5: std::cout << "Not implemented yet" << std::endl; break;
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
		state->PC = (opcode[1] << 8) | (opcode[2] & 0xff);
		break;
	}
	case 0xce: std::cout << "Not implemented yet" << std::endl; break;
	case 0xcf: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd0: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd1: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd2: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd3: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd5: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd6: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd7: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xd9: std::cout << "Not implemented yet" << std::endl; break;
	case 0xda: std::cout << "Not implemented yet" << std::endl; break;
	case 0xdb: std::cout << "Not implemented yet" << std::endl; break;
	case 0xdc: std::cout << "Not implemented yet" << std::endl; break;
	case 0xdd: std::cout << "Not implemented yet" << std::endl; break;
	case 0xde: std::cout << "Not implemented yet" << std::endl; break;
	case 0xdf: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe0: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe1: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe2: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe3: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe5: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe6: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe7: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xe9: std::cout << "Not implemented yet" << std::endl; break;
	case 0xea: std::cout << "Not implemented yet" << std::endl; break;
	case 0xeb: std::cout << "Not implemented yet" << std::endl; break;
	case 0xec: std::cout << "Not implemented yet" << std::endl; break;
	case 0xed: std::cout << "Not implemented yet" << std::endl; break;
	case 0xee: std::cout << "Not implemented yet" << std::endl; break;
	case 0xef: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf0: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf1: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf2: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf3: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf4: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf5: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf6: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf7: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf8: std::cout << "Not implemented yet" << std::endl; break;
	case 0xf9: std::cout << "Not implemented yet" << std::endl; break;
	case 0xfa: std::cout << "Not implemented yet" << std::endl; break;
	case 0xfb: std::cout << "Not implemented yet" << std::endl; break;
	case 0xfc: std::cout << "Not implemented yet" << std::endl; break;
	case 0xfd: std::cout << "Not implemented yet" << std::endl; break;
	case 0xfe: std::cout << "Not implemented yet" << std::endl; break;
	case 0xff: std::cout << "Not implemented yet" << std::endl; break;
	}
	state->PC += 1;
}
