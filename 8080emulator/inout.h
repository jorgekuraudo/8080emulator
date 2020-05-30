#pragma once
#include <iostream>

//data bus (for keyboard input)
struct bus
{
	uint8_t port1{ 0x00 };
	uint8_t port2{ 0x00 };
} bus;

//hardware shift register
struct shift_register
{
	uint16_t status = 0x0000;
	uint8_t result = 0x00;
	uint8_t offset{};
} shift16;