#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <cstring>
#include "Parser.h"
#include "CreateInst.h"
#include "InsertInst.h"


//---------------------------
// InstructionSet ParseAllInstructions(fstream)
//		Parse all input instructions into single instructions set.
//---------------------------
InstructionSet* Parser::ParseAllInstructions(fstream* inputFile)
{
	InstructionSet* instructionSet = new InstructionSet ();
	string inputString;
	string* instructionBuffer = new string();
	char* trying;

	while (getline (*inputFile, inputString)) {
		char charBuffer[1000];
		strcpy(charBuffer, inputString.c_str());
		bool instructionSlicingFlag = false;
		trying = strtok (charBuffer," \n"); //忽略縮排
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
	    	Instruction* instruction = new Instruction ();
	    	instruction->setInstructionString (*instructionBuffer);
	    	instructionSet->pushInstruction (*instruction);
	    	//cout << instructionSet->popInstruction().getInstructionString () << endl;
	    	instructionBuffer = new string ();
	    	instructionSlicingFlag = false;
	    }
	}

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
	
	//Instruction* instruction = new Instruction();
	CreateInst *instruction = new CreateInst();
	instruction->tableName = "TABLE~~~~~~~~";
	return instruction;
}