#include <iostream>
#include <Instruction.h>

Instruction::Instruction()
{
	isLegal = false;
}

Instruction::~Instruction()
{
	termTokens->clear();
	delete termTokens;
}

void Instruction::setInstructionString(string s)
{
	instructionString = s;
}

string Instruction::getInstructionString()
{
	return instructionString;
}

void Instruction::setTermTokens(vector<string> v*)
{
	termTokens = v;
}

vector<string> Instruction::getTermTokens()
{
	return termTokens;
}