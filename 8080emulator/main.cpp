#include <iostream>
#include <thread>
#include "disassembler.h"
#include "registers.h"
#include "emulateOpCode.h"
#include "screen.h"

int main(int charc, char* args[]) {

	init();

	//open the file
	const char* path = "C:/Users/jmm_1/Desktop/invadersrom/invaders";
	try {
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
		do {
			emulateOpCode(CPU_state);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
	
			if (CPU_state->isOn) {
				interrupt(CPU_state, 2);
			}
		} while (true);
		screen.join();
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	close();
	return 0;
}