#pragma once
#include <iostream>
#include "condition_flags.h"

struct State8080 {
	//general purpose registers
	uint8_t A{};
	uint8_t B{};
	uint8_t C{};
	uint8_t D{};
	uint8_t E{};
	uint8_t H{};
	uint8_t L{};

	//registers pairs (useful for many instructions)
	uint16_t HL_pair{};
	uint16_t BC_pair{};
	uint16_t DE_pair{};

	//function to make pairs
	uint16_t pair(const uint8_t& reg1, const uint8_t& reg2) {
		return (((uint16_t)reg1 << 8) | (uint16_t)reg2);
	}
	
	//stack pointer
	uint16_t SP{ 0x0000 };

	//program counter
	uint16_t PC{ 0x0000 };

	//pointer to where the game is loaded
	uint8_t* memory{};

	//condition flags
	Flags flag;

	//interruptor
	bool isOn;

	//number of intructions
	int count{};
};
State8080* state = new State8080;
