
#include <stdio.h>
#include <bitset>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Assembler.h"
#include "ConsoleLogger.h"
#include "Utilities.h"

using std::vector;
using std::string;


int main(int argc, char* argv[])
{
	std::ifstream input("INPUT", std::ios::in);

	vector<string> fileAsLines;
	string currentLine;
	while (std::getline(input, currentLine))
		fileAsLines.push_back(currentLine);

	input.close();

	std::cout << "Recieved the following raw input: " << std::endl;

	for (string const& line : fileAsLines)
		std::cout << line << '\n';

	Assembler::RemoveAllTextAfterAndIncludingENDMacro(fileAsLines);

	for (string& line : fileAsLines) 
		Assembler::RemoveCommentsFromLine(line);

	vector<vector<string>> tokenizedInput = Assembler::GetTokenizedInputStrings(fileAsLines);
	
	Assembler::ResolveAndReplaceLabels(tokenizedInput);

	std::cout << "Recieved the following parsed input: " << std::endl;

	for (size_t lineIndex = 0; lineIndex < tokenizedInput.size(); ++lineIndex) 
	{
		std::cout << Utilities::ConcatenateStrings(tokenizedInput[lineIndex], ' ') << std::endl;
	}



	vector<uint16_t> outputOfAssembler = Assembler::AssembleIntoBinary(tokenizedInput);
	
	if (Assembler::AreErrors())
	{
		ConsoleLogger logger = ConsoleLogger();

		Assembler::LogErrors(logger);
		return 1;
	}
	else 
	{
		std::cout << "No errors were encountered during assembly.";
	}

	for (uint16_t& value : outputOfAssembler)
	{
		value = Utilities::SwitchEndianness(value);
	}


	std::ofstream output("ASSEMBLY", std::ios::binary | std::ios::trunc);

	if (output.is_open())
	{
		output.write(reinterpret_cast<char*>(&outputOfAssembler[0]), outputOfAssembler.size() * sizeof(uint16_t));
		
		output.close();

		std::cout << "Assembly complete." << std::endl;
	}





	return 0;
}