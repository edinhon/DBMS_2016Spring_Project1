#include <iostream>
#include "Instruction.h"

Instruction::Instruction()
{
	isValid = true;
}

Instruction::~Instruction()
{
	//termTokens->clear();
	//delete termTokens;
}

//----------------
// void setInstructionString(string)
//		Set complete single instruction string.
//----------------
void Instruction::setInstructionString(string s)
{
	instructionString = s;
}

//----------------
// string getInstructionString()
//		Get complete single instruction string (to parse into tokens).
//----------------
string Instruction::getInstructionString()
{
	return instructionString;
}

//----------------
// void setTermTokens(vector<string>);
//		Set tokens from instruction string.
//----------------
void Instruction::setTermTokens(vector<string> *v)
{
	termTokens = v;
}

//----------------
// vector<string> getTermTokens();
//		Get tokens to run this instruction.
//----------------
vector<string> Instruction::getTermTokens()
{
	return *termTokens;
}