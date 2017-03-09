#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Instruction.h>
#include <InstructionSet.h>

using namespace std;

class Parser {
	public:
		
		Parser();
		
		//---------------------------
		// InstructionSet ParseAllInstructions(fstream)
		//		Parse all input instructions into single instructions set.
		//---------------------------
		InstructionSet ParseAllInstructions(fstream inputFile);
		
		//---------------------------
		// Instruction ParseSingleInstruction(Instruction)
		//		Parse single instruction into instruction terms set.
		//---------------------------
		Instruction ParseSingleInstruction(Instruction c);
		
	private:
		
}
