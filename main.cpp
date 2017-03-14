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

void DBMS(string fileName);
bool ChooseInputFileOrNot();
TableSet tableSet;

int main () {
	
	if (ChooseInputFileOrNot()){
		string fileName;
		cout << "Please input the file name >>> ";
		std::getline(cin, fileName);
		DBMS(fileName);
	} else {
		string inputS;
		cout << "Please input the instruction :\n" << ">>> ";
		while(std::getline (cin, inputS, ';')){
			fstream fp;
			fp.open("InputTemp.dbms", ios::out);
			fp << inputS << endl;
			fp.close();
			DBMS("InputTemp.dbms");
			cout << ">>> ";
		}
	}
	
	return 0;
}

void DBMS(string fileName)
{
	fstream* fp= new fstream();
	fp->open (fileName, ios::in);
	if (!fp) {
		cout << "Error: Cannot open the file " << fileName << endl;
		return;
	}

	Parser parser;
	InstructionSet* instructionSet;
	//TableSet tableSet;
	
	instructionSet = parser.ParseAllInstructions(fp);
	
	while(!instructionSet->isEmpty()){
		// fetch instructions
		Instruction instruction = instructionSet->fetchInstruction();
		Instruction *inst = parser.ParseSingleInstruction(instruction);
		
		if (!inst->isValid) {
			instructionSet->popInstruction();
			continue;
		}

		switch(inst->type){
			case CREATE :{
				CreateInst *cinst = dynamic_cast<CreateInst*>(inst);
				
				if (tableSet.CheckDuplicateTable(cinst)){
					Table t(cinst);
					tableSet.PushTable(t);
				}
				tableSet.ShowTables();
				break;
			}
			case INSERT :{
				InsertInst *iinst = dynamic_cast<InsertInst*>(inst);

				int tableIndex = tableSet.SearchTable(iinst->tableName);
				if (tableIndex != -1){
					Table *t = tableSet.GetTable(tableIndex);
					if(!t->CheckInsertInst(iinst)) break;
					t->InsertTuple(iinst);
				}
				tableSet.ShowTables();
				break;
			}
			case SELECT :{
				
				break;
			}
		}
		
		instructionSet->popInstruction ();
	}

	fp->close ();
}

bool ChooseInputFileOrNot()
{
	cout << "Do you want to input from a file?[Y/N] ";
	string choose;
	std::getline(cin, choose);
	if(choose.compare("Y") == 0 || choose.compare("y") == 0) return true;
	else if (choose.compare("N") == 0 || choose.compare("n") == 0) return false;
	else return false;
}


