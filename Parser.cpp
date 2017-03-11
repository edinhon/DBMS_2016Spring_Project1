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
	char* trying;

	while (getline (*inputFile, inputString, ';')) {
		string slicedString = "\0";
		char charBuffer[1000];
		strcpy(charBuffer, inputString.c_str());
		trying = strtok (charBuffer," \n"); //忽略縮排
		while (trying != NULL) { 
			//printf("%s\n", trying);
			string stringBuffer (trying);
			slicedString += " ";
			slicedString += stringBuffer;
	        trying = strtok (NULL, " \n");
		}
		Instruction* instruction = new Instruction();
		instruction->setInstructionString(slicedString);
		instructionSet->pushInstruction(*instruction);
	}

	return instructionSet;
}

//---------------------------
// Instruction ParseSingleInstruction(Instruction)
//		Parse single instruction into instruction terms set.
//---------------------------
void Parser::ParseSingleInstruction(Instruction instruction)
{
	//TODO: Decide which type of this instruction, 
	//	and new a corresponding instruction class, 
	//	and push it into instructionSet.
	char charBuffer[1000];
	strcpy(charBuffer, instruction.getInstructionString().c_str());
	char* trying;
	//bool instructionSlicingFlag = false;
	trying = strtok (charBuffer," ,"); //忽略縮排
	while (trying != NULL) {
		//cout << trying << endl;
		//instruction.setInstructionString ()
	    trying = strtok (NULL, " ,");
	}

	return;
	
	//Instruction* instruction = new Instruction();
	/*
	CreateInst *instruction = new CreateInst();
	instruction->tableName = "TABLE~~~~~~~~";
	return instruction;
	*/
}