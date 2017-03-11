#include <iostream>
#include "Instruction.h"

Instruction::Instruction()
{
	isLegal = false;
}

Instruction::Instruction (Instruction const &that)
{
	isLegal = that.isLegal;
	type = that.type;
	instructionString = that.instructionString;
	termTokens = that.termTokens;
}

Instruction::~Instruction()
{
	//termTokens->clear();
	//delete termTokens;
}

Instruction& Instruction::operator= (Instruction const &that)
{
	if (this != &that) {
		isLegal = that.isLegal;
		type = that.type;
		instructionString = that.instructionString;
		termTokens = that.termTokens;
	}
	return *this;
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
void Instruction::setTermTokens(vector<string> v)
{
	termTokens = v;
}

//----------------
// vector<string> getTermTokens();
//		Get tokens to run this instruction.
//----------------
vector<string> Instruction::getTermTokens()
{
	return termTokens;
}