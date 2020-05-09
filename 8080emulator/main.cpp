#include <iostream>
#include "disassembler.h"

int main() {
	//open the file
	/*const char* path = "C:/Users/jmm_1/Downloads/invaders.h";
	try {
		disassembler dis(path);
		dis.automate();
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
	
	return 0;*/

	std::vector<std::string> prueba{ disassembler::splitString("0x01	LXI B,D16	3		B <- byte 3, C <- byte 2") };
	for (auto elem : prueba) {
		std::cout << elem << '-';
	}
}