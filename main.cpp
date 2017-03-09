#include <iostream>
#include <Parser.h>
#include <Command.h>
#include <CommandSet.h>

using namespace std;

enum class InstructionType {CREATE, INSERT, SELECT};

int main () {

	fstream input_testcase;
	Parser parser = new Parser();
	InstructionSet instructionSet = new InstructionSet();
	
	input_testcase.open ("input testcase.txt", ios::in);
	instructionSet = parser.ParseAllInstructions(input_testcase);
	
	Instruction instruction;
	while(!instructionSet.isEmpty()){
		instruction = parser.ParseSingleInstruction(instructionSet.popInstruction());
		
		switch(instruction.type){
			case InstructionType::CREATE :
				
				break;
			case InstructionType::INSERT :
				
				break;
			case InstructionType::SELECT :
				
				break;
		}
		//TODO: Deal with the instruction.
	}
	
	input_testcase.close ();
	return 0;
}


