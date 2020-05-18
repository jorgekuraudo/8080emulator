#include <iostream>
#include <thread>
#include "disassembler.h"
#include "registers.h"
#include "emulateOpCode.h"
#include "screen.h"

int main(int charc, char* args[]) {

	init(); //initialize window

	//open the file
	const char* path = "C:/Users/jmm_1/Desktop/invadersrom/invaders";

	disassembler dis(path);
	//dis.disassemble();
	std::vector<uint8_t> codes = dis.getInstructions();

	//rest of RAM
	for (int i = 0; i < 0x4000; ++i) {
		codes.push_back(0xff);
	}

	//create a model of the CPU
	State8080* CPU_state = new State8080;

	//initialize some CPU parameters	
	CPU_state->memory = &codes[0];
	CPU_state->isOn = false;
	CPU_state->count = 0;
	CPU_state->PC = 0;
	CPU_state->A = 0;
	CPU_state->B = 0;
	CPU_state->C = 0;
	CPU_state->D = 0;
	CPU_state->E = 0;
	CPU_state->H = 0;
	CPU_state->L = 0;

	//set the screen
	std::thread screen([&]() {
		while (true) {
			updateScreen(&CPU_state->memory[0x2400]);
		}
		});

	//emulation starts here
	do {
		emulateOpCode(CPU_state);

		if (CPU_state->isOn) {
			if (gCurrentByte == 3000) {
				interrupt(CPU_state, 1);
			}
			if (gCurrentByte == 7000) {
				interrupt(CPU_state, 2);
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym)
				{
				case SDLK_c: (bus.port1 | 0x01); std::cout << "coin" << std::endl; break; //insert coin
				case SDLK_1: (bus.port1 | 0x02); std::cout << "p1" << std::endl; break; //start P1
				case SDLK_2: (bus.port1 | 0x04); std::cout << "p2" << std::endl; break; // start P2
				}
			}
			else if (e.type == SDL_KEYUP) {

			}
		}
	} while (true);
	screen.join();

	close();
	return 0;
}