#include <fstream>
#include <vector>
#include <iostream>
#include "Utilities.h"

using std::vector;

uint16_t Utilities::LoadFileInto(string filename, uint16_t* memory, int memorySize, bool swapEndianness)
{
	std::ifstream input(filename, std::ios::in | std::ios::binary);

	if (!input.is_open())
	{
		std::cout << "LOAD (did) FAILED" << std::endl;
		return 0;
	}

	uint16_t startAddress;
	input.read(reinterpret_cast<char*>(&startAddress), 2);

	input.seekg(0, input.end);
	uint16_t lengthOfFile = input.tellg() / 2;
	input.seekg(0, input.beg);

	if (swapEndianness)
		startAddress = startAddress << 8 | startAddress >> 8;

	std::cout << "Start address read as " << startAddress << std::endl;
	

	
	std::cout << "Length of file read as: " << std::to_string(lengthOfFile) << " words" << std::endl;

	if (startAddress + lengthOfFile < memorySize) 
	{
		std::cout << "File shorter than available space. Reading " << std::to_string(lengthOfFile) << " units instead." << std::endl;
	} 
	else 
	{
		std::cout << "File larger than available space. Aborting..." << std::endl;
		input.close();
		return 0;
	}

	uint16_t buffer;

	for (size_t i = 0; i < lengthOfFile; ++i)
	{
		input.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
		if (swapEndianness)
		{
			buffer = buffer << 8 | buffer >> 8;
		}
		memory[startAddress + i] = buffer;
	}

	input.close();

	if (!input.good())
	{
		perror("LOAD (not) OK");
	}

	std::cout << "File done being read." << std::endl;


	return startAddress;
}
