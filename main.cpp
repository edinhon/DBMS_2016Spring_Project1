#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Instruction.h"
#include "InstructionSet.h"

using namespace std;

enum InstructionType {CREATE, INSERT, SELECT};

int main () {

	fstream* input_testcase= new fstream();
	input_testcase->open ("input testcase.txt", ios::in);

	Parser parser;
	InstructionSet* instructionSet;// = new InstructionSet();
	
	instructionSet = parser.ParseAllInstructions(input_testcase);
	
	while(!instructionSet->isEmpty()){

		Instruction* instruction = parser.ParseSingleInstruction(instructionSet->popInstruction());
		
		switch(instruction->type){
			case CREATE :
				
				break;
			case INSERT :
				
				break;
			case SELECT :
				
				break;
		}
		//TODO: Deal with the instruction.
	}
	
	input_testcase->close ();
	return 0;
}


