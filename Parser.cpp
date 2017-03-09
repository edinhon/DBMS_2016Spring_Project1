#include <iostream>
#include "Parser.h"


//---------------------------
// InstructionSet ParseAllInstructions(fstream)
//		Parse all input instructions into single instructions set.
//---------------------------
InstructionSet* Parser::ParseAllInstructions(ifstream* inputFile)
{
	InstructionSet* instructionSet = new InstructionSet ();
	string inputstring;
	//while (getline (inputstring))

	return instructionSet;
}

//---------------------------
// Instruction ParseSingleInstruction(Instruction)
//		Parse single instruction into instruction terms set.
//---------------------------
Instruction* Parser::ParseSingleInstruction(Instruction c)
{
	//TODO: Decide which type of this instruction, 
	//	and new a corresponding instruction class, 
	//	and push it into instructionSet.
	Instruction* instruction = new Instruction();
	return instruction;
}