#include <iostream>
#include "disassembler.h"

int main() {
	//open the file
	const char* path = "C:/Users/jmm_1/Desktop/invadersrom/invaders";
	try {
		disassembler dis(path);
		dis.disassemble();
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}