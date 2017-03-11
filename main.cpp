#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Instruction.h"
#include "InstructionSet.h"
#include "CreateInst.h"
#include "InsertInst.h"
#include "Table.h"
#include "TableSet.h"

using namespace std;

enum InstructionType {CREATE, INSERT, SELECT};

int main () {

	fstream* input_testcase= new fstream();
	input_testcase->open ("input testcase.txt", ios::in);

	Parser parser;
	InstructionSet* instructionSet;
	TableSet tableSet;
	
	instructionSet = parser.ParseAllInstructions(input_testcase);
	
	while(!instructionSet->isEmpty()){
		// fetch instructions
		Instruction instruction = instructionSet->fetchInstruction();
		cout << instruction.getInstructionString () << endl;
		parser.ParseSingleInstruction(instruction);
		
		switch(instruction.type){
			case CREATE :
				
				break;
			case INSERT :
				
				break;
			case SELECT :
				
				break;
		}
		//TODO: Deal with the instruction.
		instructionSet->popInstruction ();
	}
	
	input_testcase->close ();
	return 0;
}


