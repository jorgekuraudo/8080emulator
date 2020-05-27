#pragma once
#include <iostream>

struct Flags {
	bool Z{ false };
	bool S{ false };
	bool P{ false };
	bool C{ false };
	bool AC{ false };
};

//for parity flag calculation
bool parity(uint16_t bytes) {
	int bit_counter{ 0 };
	for (int i = 0; i < 16; ++i)
	{
		((bytes >> i) & 1) == 1 ? ++bit_counter : bit_counter;
	}
	if (bit_counter % 2 == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}