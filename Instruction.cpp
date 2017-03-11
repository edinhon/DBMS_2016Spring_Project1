#include <iostream>
#include "Instruction.h"

Instruction::Instruction()
{
	isValid = true;
}

Instruction::Instruction (Instruction const &that)
{
	isValid = that.isValid;
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
		isValid = that.isValid;
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
void Instruction::setTermTokens(string term)
{
	termTokens.push(term);
}

//----------------
// vector<string> getTermTokens();
//		Get tokens to run this instruction.
//----------------
string Instruction::getTermTokens()
{
	string tmpt = termTokens.front();
	termTokens.pop();
	return tmpt;
}

bool checkStringWithoutCase (string A, string B)
{
	bool trueOfFalse = true;
	if (A.size() != B.size())
		return false;

	for (int i=0; i<A.size(); i++) {
		if (A[i] == B[i])
			continue;
		else if (A[i]+32 == B[i])
			continue;
		else if (B[i]+32 == A[i])
			continue;
		else
			return false;
	}
	return trueOfFalse;
}
bool checkIfIsNumber (string A)
{
	for (int i=0; i<A.size(); i++)
		if (A[i] < '0' || A[i] > '9')
			return false;

	return true;
}