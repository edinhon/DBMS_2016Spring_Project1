#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <cstring>
#include "Parser.h"


//---------------------------
// InstructionSet ParseAllInstructions(fstream)
//		Parse all input instructions into single instructions set.
//---------------------------
InstructionSet* Parser::ParseAllInstructions(fstream* inputFile)
{
	InstructionSet* instructionSet = new InstructionSet ();
	Instruction* instruction = new Instruction ();
	string inputString;
	string* instructionBuffer = new string();
	vector <string> instructionStream;
	char* trying;

	while (getline (*inputFile, inputString)) {
		char charBuffer[1000];
		strcpy(charBuffer, inputString.c_str());
		bool instructionSlicingFlag = false;
		trying = strtok (charBuffer," \n");
		while (trying != NULL) {
	    	string stringBuffer(trying);
	    	*instructionBuffer += " ";
	    	*instructionBuffer += stringBuffer;
	    	if (trying[strlen(trying)-1] == ';') {	//抓分號
				instructionSlicingFlag = true;
				break;
			}
	        trying = strtok (NULL, " \n");
	    }
	    if (instructionSlicingFlag) {

	    	instruction->setInstructionString (*instructionBuffer);
	    	cout << instruction->getInstructionString () << endl;
	    	//instructionSet->pushInstruction (*instruction);
	    	
	    	instructionBuffer = new string ();
	    	instruction = new Instruction ();
	    	instructionSlicingFlag = false;
	    }
	}
	
	//for (int i=0; i<instructionStream.size(); i++)
	//	cout << instructionStream[i] << endl << '\n';

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