#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define CREATE_TABLE 0
#define INSERT_TUPLE 1
#define SELECTION 2

#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

class Instruction {
	public:
		
		Instruction();
		Instruction (Instruction const &that);
		
		virtual ~Instruction();
		
		Instruction& operator= (Instruction const &that);

		//----------------
		// void setInstructionString(string)
		//		Set complete single instruction string.
		//----------------
		void setInstructionString(string s);
		
		//----------------
		// string getInstructionString()
		//		Get complete single instruction string (to parse into tokens).
		//----------------
		string getInstructionString();
		
		//----------------
		// void setTermTokens(vector<string>);
		//		Set tokens from instruction string.
		//----------------
		void setTermTokens(string);
		
		//----------------
		// vector<string> getTermTokens();
		//		Get tokens to run this instruction.
		//----------------
		string getTermTokens();
		void popTermTokens ();
		bool isEmpty ();

		bool isValid;
		
		int type;

		
	private:
		
		queue<string> termTokens;		
		string instructionString;
		
};

bool checkStringWithoutCase (string, string);
bool checkIfIsNumber (string);
#endif