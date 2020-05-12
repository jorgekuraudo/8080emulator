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