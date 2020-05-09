#include "disassembler.h"

disassembler::disassembler(const char* _path) {
	path = _path;
	file.open(path, std::ifstream::binary);
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
	char currentByte[1];
	int offset{ 0 };
	//open file with opcodes
	std::ifstream opcodes;
	opcodes.open("C:/Users/jmm_1/Desktop/opcodes.txt");

	//create a struct to store the opcodes
	struct codes
	{
		int instruction_code;
		std::string instruction;
		int instruction_size;
	};
	std::vector<codes> all_codes;
	std::string line;
	while (std::getline(opcodes, line)) {
		std::vector<std::string> temp;
		std::string temp_string;
	}
	while (file.peek() != EOF) {
		file.read(currentByte, 1);
		file.seekg(offset);
		short opBytes{ 1 };
		
		


		offset += opBytes;
	}
}

//work a little bit more on this
std::vector<std::string> disassembler::splitString(std::string line) {
	std::vector<std::string> result;
	std::string temp{ "" };
	for (char c : line) {
		if (!isblank(c)) {
			temp += c;
		}
		else {
			result.push_back(temp);
			temp.clear();
		}
	}
	return result;
}