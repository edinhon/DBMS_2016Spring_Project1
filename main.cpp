#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Instruction.h"
#include "InstructionSet.h"
#include "CreateInst.h"
#include "InsertInst.h"
#include "SelectInst.h"
#include "CreateIndexInst.h"
#include "Table.h"
#include "TableSet.h"
#include <algorithm>

using namespace std;

enum InstructionType {CREATE, INSERT, SELECT, CREATEIDX};

void DBMS(string fileName);
bool ChooseInputFileOrNot();
TableSet tableSet;

int main () {
	
	DBMS ("input testcase.txt");
	/*
	if (ChooseInputFileOrNot()){
		string fileName;
		cout << "Please input the file name >>> ";
		std::getline(cin, fileName);
		DBMS(fileName);
	} else {
		string inputS;
		cout << "Please input the instruction or type in  \" exit; \" to exit):\n" << ">>> ";
		while(std::getline (cin, inputS, ';')){
			fstream fp;
			fp.open("InputTemp.dbms", ios::out);
			fp << inputS << endl;
			fp.close();
			DBMS("InputTemp.dbms");
			cout << ">>> ";
		}
	}
	*/
	return 0;
}


void DBMS(string fileName)
{
	tableSet.InformationRead_TableSet();
	tableSet.LoadTableSet ();
//	tableSet.ShowTables ();
	
	fstream* fp = new fstream();
	fp->open (fileName, ios::in);
	if (!fp) {
		cout << "- Error: Cannot open the file " << fileName << endl;
		return;
	}

	Parser parser;
	InstructionSet* instructionSet;
	
	instructionSet = parser.ParseAllInstructions(fp);
	
	while(!instructionSet->isEmpty()){
		
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
				//tableSet.ShowTables();
				//cout << endl << "============================" << endl;
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
				//tableSet.ShowTables();
				break;
			}
			case SELECT :{
				SelectInst *sinst = dynamic_cast<SelectInst*>(inst);
				
				if(tableSet.ContainTables(sinst->tableNames)){
					if(tableSet.SelectTable(sinst)){
						Table *t = tableSet.GetSelectedTable();
						t->ShowTable(sinst);
						
						if(sinst->isCOUNT){
							t->Count_ShowTable(sinst);
						}
						if(sinst->isSUM){
							t->Sum_ShowTable(sinst);
						}
						tableSet.DeleteSelectedTable();
					}
				}
				//cout << "end select" << endl;
				break;
			}
			case CREATEIDX :{
				CreateIndexInst *ciinst = dynamic_cast<CreateIndexInst*>(inst);
				
				int tableIndex = tableSet.SearchTable(ciinst->tableName);
				if (tableIndex != -1){
					Table *t = tableSet.GetTable(tableIndex);
					t->CreateIndex(ciinst);
				}
				
				break;
			}
		}
		
		cout << endl << "============================" << endl;
		
		instructionSet->popInstruction ();
	}
	//tableSet.ShowTables ();
	fp->close ();
	tableSet.InformationWrite_TableSet ();
}

bool ChooseInputFileOrNot()
{
	cout << "Do you want to input from a file?[Y/N] : ";
	string choose;
	std::getline(cin, choose);
	if(choose.compare("Y") == 0 || choose.compare("y") == 0) return true;
	else if (choose.compare("N") == 0 || choose.compare("n") == 0) return false;
	else return false;
}
