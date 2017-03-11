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
		//cout << "I am in" << endl;
		//Instruction* instruction = parser.ParseSingleInstruction(instructionSet->popInstruction());
		//Instruction instruction = instructionSet->popInstruction();
		//cout << instruction.getInstructionString () << endl;
		
		Instruction *inst = parser.ParseSingleInstruction(instructionSet->popInstruction());
		if(inst->isValid){
			switch(inst->type){
				case CREATE :{
					//cast down : From Instruction to CreateInst
					CreateInst *cinst = dynamic_cast<CreateInst*> (inst);
					Table t(cinst);
					tableSet.PushTable(t);
					cout << t.getTableName() << "=" << tableSet.GetTable(0)->getTableName() << endl;
					break;
				}
				case INSERT :{
					
					break;
				}
				case SELECT :{
					
					break;
				}
			}
			//TODO: Deal with the instruction.
		}
		
	}
	
	input_testcase->close ();
	return 0;
}


