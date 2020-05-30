#include <iostream>
#include "disassembler.h"
#include "cpu.h"
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

	//RAM
	for (int i = 0x1fff; i < 0xffff; ++i) {
		codes.push_back(0x55);
	}

	//initialize some CPU parameters	
	state->memory = &codes[0];
	state->isOn = false;

	//////offset for debugging
	//for (int off = 0; off < 0x100; ++off) {
	//	codes.insert(codes.begin(), 0);
	//}

	///*Fix the stack pointer from 0x6ad to 0x7ad
	// this 0x06 byte 112 in the code, which is
	// byte 112 + 0x100 = 368 in memory  */
	//state->memory[0x368] = 0x3F;

	////Skip DAA test    
	//state->memory[0x59c] = 0xc3; //JMP    
	//state->memory[0x59d] = 0xc2;
	//state->memory[0x59e] = 0x05;

	/*CPU_state->memory[0x528] = 0xc3;
	state->memory[0x529] = 0x36;
	state->memory[0x530] = 0x5;*/


	////Fix the first instruction to be JMP 0x100    
	//state->memory[0] = 0xc3;
	//state->memory[1] = 0;
	//state->memory[2] = 0x01;

	//emulation starts here
	myTimer->start();
	int alter{ 0 }; // to alternate between interrupts

	while(true) {
		emulateOpCode(state);

		if ((state->isOn) && (myTimer->readTime() > 17)) {
			if (alter % 2 == 0) {
				logFile << "===========interrupt one===========\n";
				interrupt(state, 1);
				updateUpperHalf(&state->memory[0x2400]);
				state->isOn = true; //disable interrupt
			}
			else {
				logFile << "===========interrupt two===========\n";
				interrupt(state, 2);
				updateBottomHalf(&state->memory[0x2400]);
				state->isOn = true; //disable interrupt
			}
			myTimer->reset();
			++alter;
		}
		

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym)
				{
				case SDLK_c: (bus.port1 = bus.port1 | 0x01); std::cout << "coin" << std::endl; break; //insert coin
				case SDLK_2: (bus.port1 = bus.port1 | 0x02); std::cout << "p2" << std::endl; break; //start P2
				case SDLK_1: (bus.port1 = bus.port1 | 0x04); std::cout << "p1" << std::endl; break; // start P1
				case SDLK_s: (bus.port1 = bus.port1 | 0x10); std::cout << "P1 shoot" << std::endl; break; // P1 shoot
				case SDLK_a: (bus.port1 = bus.port1 | 0x20); std::cout << "P1 left" << std::endl; break; // P1 left
				case SDLK_d: (bus.port1 = bus.port1 | 0x40); std::cout << "P1 right" << std::endl; break; // P1 right
				}
			}
			else if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym)
				{
				case SDLK_c: (bus.port1 = bus.port1 & 0xfe); break; //insert coin
				case SDLK_2: (bus.port1 = bus.port1 & 0xfd); //start P2
				case SDLK_1: (bus.port1 = bus.port1 & 0xfb); // start P1
				case SDLK_s: (bus.port1 = bus.port1 & 0xef); // P1 shoot
				case SDLK_a: (bus.port1 = bus.port1 & 0xdf); // P1 left
				case SDLK_d: (bus.port1 = bus.port1 & 0xbf); // P1 right
				}
			}
		}
	}

	//clean
	close();
	logFile.close();
	delete myTimer;
	return 0;
}