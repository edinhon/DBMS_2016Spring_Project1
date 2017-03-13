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
		Instruction *inst = parser.ParseSingleInstruction(instruction);
		
		switch(inst->type){
			case CREATE :{
				CreateInst *cinst = dynamic_cast<CreateInst*>(inst);
				
				if (tableSet.CheckDuplicateTable(cinst)){
					Table t(cinst);
					tableSet.PushTable(t);
					//cout << t.getTableName() << endl;
				}
				break;
			}
			case INSERT :{
				InsertInst *iinst = dynamic_cast<InsertInst*>(inst);
				
				int tableIndex = tableSet.SearchTable(iinst->tableName);
				if (tableIndex != -1){
					Table *t = tableSet.GetTable(tableIndex);
					
					if(t->CheckInsertInst(iinst)) break;
					
					t->InsertTuple(iinst);
				}
				break;
			}
			case SELECT :{
				
				break;
			}
		}
		//TODO: Deal with the instruction.
		instructionSet->popInstruction ();
	}
	
	input_testcase->close ();
	return 0;
}


