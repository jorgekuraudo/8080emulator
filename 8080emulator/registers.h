#pragma once
#include <iostream>
#include "condition_flags.h"

struct State8080 {
	//general purpose registers
	uint8_t A;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t H;
	uint8_t L;

	//stack pointer
	uint16_t SP;

	//program counter
	uint16_t PC;

	//pointer to where the game is loaded
	uint8_t* memory;

	//video RAM
	uint8_t* video;

	//condition flags
	Flags flag;

	//interruptor
	bool isOn;

	//number of intructions
	int count;
};
