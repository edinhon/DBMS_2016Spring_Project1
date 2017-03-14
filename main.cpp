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
		//cout << "*************\nstart parsing" << endl;
		Instruction *inst = parser.ParseSingleInstruction(instruction);
		
		if (!inst->isValid) {
			instructionSet->popInstruction();
			//cout << "Error: Syntax error" << endl;
			continue;
		}

		switch(inst->type){
			case CREATE :{
				//cout << "create table" << endl;

				CreateInst *cinst = dynamic_cast<CreateInst*>(inst);
				
				if (tableSet.CheckDuplicateTable(cinst)){
					Table t(cinst);
					tableSet.PushTable(t);
				}
				tableSet.ShowTables();
				break;
			}
			case INSERT :{
				//cout << "++++++++++++++\n"<<"insert table" << endl;
				InsertInst *iinst = dynamic_cast<InsertInst*>(inst);
				
				int tableIndex = tableSet.SearchTable(iinst->tableName);
				//cout << ">>>end search table" << endl;
				if (tableIndex != -1){
					Table *t = tableSet.GetTable(tableIndex);
					
					if(!t->CheckInsertInst(iinst)) break;
					//cout << ">>>return true" << endl;
					t->InsertTuple(iinst);
				}
				//tableSet.ShowTables();
				//cout << "---------------\n"<<"finish insert table" << endl;
				break;
			}
			case SELECT :{
				
				break;
			}
		}
		
		instructionSet->popInstruction ();
	}

	input_testcase->close ();
	return 0;
}
