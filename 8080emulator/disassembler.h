#pragma once
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <iomanip>

class disassembler
{
private:
	const char* path;
	std::ifstream file;
public:
	disassembler(const char* _path);

	~disassembler();

	void printRawData();

	void disassemble();

	static std::vector<std::string> splitString(std::string line, char ch);
};

