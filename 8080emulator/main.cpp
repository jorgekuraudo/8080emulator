#include <iostream>
#include "disassembler.h"
#include "registers.h"
#include "emulateOpCode.h"
#include "screen.h"
#include "Timer.h"

int main(int charc, char* args[]) {

	init(); //initialize window
	Timer* myTimer = new Timer; //initialize a Timer object

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

	////Fix the stack pointer from 0x6ad to 0x7ad    
	//// this 0x06 byte 112 in the code, which is    
	//// byte 112 + 0x100 = 368 in memory    
	//CPU_state->memory[368] = 0x7;

	////Skip DAA test    
	//CPU_state->memory[0x59c] = 0xc3; //JMP    
	//CPU_state->memory[0x59d] = 0xc2;
	//CPU_state->memory[0x59e] = 0x05;

	CPU_state->A = 0;
	CPU_state->B = 0;
	CPU_state->C = 0;
	CPU_state->D = 0;
	CPU_state->E = 0;
	CPU_state->H = 0;
	CPU_state->L = 0;

	//emulation starts here
	myTimer->start();
	int alter{ 0 }; // to alternate between interrupts
	do {
		emulateOpCode(CPU_state);

		if (CPU_state->isOn && myTimer->readTime() > 100) {
			if (alter % 2 == 0) {
				std::cout << "interrupt one" << std::endl;
				interrupt(CPU_state, 1);
				updateUpperHalf(&CPU_state->memory[0x2400]);
			}
			else {
				std::cout << "interrupt two" << std::endl;
				interrupt(CPU_state, 2);
				updateBottomHalf(&CPU_state->memory[0x2400]);
			}
			++alter;
			myTimer->reset();
		}

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

	//clean
	close();
	delete CPU_state;
	delete myTimer;
	return 0;
}