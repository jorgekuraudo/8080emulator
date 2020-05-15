#include <iostream>
#include "disassembler.h"
#include "registers.h"
#include "emulateOpCode.h"

int main() {
	//open the file
	const char* path = "C:/Users/jmm_1/Desktop/invadersrom/invaders";
	try {
		disassembler dis(path);
		//dis.disassemble();
		std::vector<uint8_t> codes = dis.getInstructions();
		//create a model of the CPU
		State8080* CPU_state = new State8080;
		//initialize some CPU parameters
		CPU_state->memory = &codes[0];
		CPU_state->PC = 0;
		CPU_state->A = 0;
		CPU_state->B = 0;
		CPU_state->C = 0;
		CPU_state->D = 0;
		CPU_state->E = 0;
		CPU_state->H = 0;
		CPU_state->L = 0;

		do {
			emulateOpCode(CPU_state);
			//printf("%02hhx ", CPU_state->PC);
		} while (CPU_state->isOn);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
	

	//Random debugging stuff
	/*State8080 state;
	state.PC = 0x00;
	std::cout << "before: " << (int)state.PC << std::endl;
	emulateOpCode(&state);
	std::cout << "after: " << (int)state.PC << std::endl;

	for (int i = 0; i <= 255; ++i) {
		std::cout << "case 0x" << std::hex << i << ": std::cout << \"Not implemented yet\" << std::endl; break;" << std::endl;
	}*/

	return 0;
}