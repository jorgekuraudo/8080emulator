#include "disassembler.h"

disassembler::disassembler(const char* _path) {
	path = _path;
	file.open(path);
	if (file.is_open()) {
		std::cout << "File " << path << " was opened" << std::endl;;
	}
	else {
		throw std::runtime_error("Couldn't open the requested file");
	}
}

disassembler::~disassembler() {
	file.close();
	std::cout << "File " << path << " was closed" << std::endl;
}

void disassembler::printRawData() {
	char currentByte[1];
	int count{ 0 };
	int bytesPerRow = 16;
	while (file.peek() != EOF) {
		for (int i = 0; i < bytesPerRow; ++i) {
			file.read(currentByte, 1);
			//std::cout << std::showbase << std::hex << currentByte[0] << std::endl;
			printf("%02hhx ", currentByte[0]);
			++count;
			file.seekg(count);
		}
		printf("\n");
	}
}

void disassembler::disassemble() {
	//open file with opcodes
	std::ifstream opcodes;
	opcodes.open("C:/Users/jmm_1/Desktop/opcodes.txt");

	//create a struct to store the opcodes
	struct codes
	{
		int instruction_code{ 0x00 };
		std::string instruction{ "" };
		int instruction_size{ 0 };
	};
	std::vector<codes> all_codes;
	std::string line;
	while (std::getline(opcodes, line)) {
		std::vector<std::string> temp{ disassembler::splitString(line, '\t') };
		codes code;
		code.instruction_code = stoi(temp[0], 0, 16);
		code.instruction = temp[1];

		//some opcodes don't have an instruction so temp[2] is undefinied
		try {
			code.instruction_size = stoi(temp[2]);
		}
		catch(std::invalid_argument& e) {
			code.instruction_size = 1;
		}
		//std::cout << "0x" << std::hex << code.instruction_code << std::endl;

		all_codes.push_back(code);
	}

	//parse the file
	char currentByte[1];
	int offset{ 0 };
	while (file.peek() != EOF) {
		file.seekg(offset);
		file.read(currentByte, 1);
		unsigned int index = (0xff & (unsigned int)currentByte[0]);
		int size = all_codes[index].instruction_size;
		
		//print intructions on screen
		std::cout << "0x" << std::hex << all_codes[index].instruction_code << ' ';
		for (int i = 1; i < size; ++i) {
			file.seekg(offset + i);
			file.read(currentByte, 1);
			std::cout << "0x" << std::hex << (0xff & (unsigned int)currentByte[0]) << ' ';
		}
		std::cout << '\r' << '\t' << '\t' << all_codes[index].instruction << '\n';
		offset += size;
	}
}

std::vector<std::string> disassembler::splitString(std::string line, char ch) {
	std::vector<std::string> result;
	std::string temp{ "" };
	for (char c : line) {
		if (c != ch) {
			temp += c;
		}
		else {
			result.push_back(temp);
			temp.clear();
		}
	}
	result.push_back(temp);
	return result;
}