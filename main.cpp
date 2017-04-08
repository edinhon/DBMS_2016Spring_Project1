#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Instruction.h"
#include "InstructionSet.h"
#include "CreateInst.h"
#include "InsertInst.h"
#include "SelectInst.h"
#include "Table.h"
#include "TableSet.h"
#include <algorithm>

using namespace std;

enum InstructionType {CREATE, INSERT, SELECT};

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
	fstream* fp= new fstream();
	fp->open (fileName, ios::in);
	if (!fp) {
		cout << "- Error: Cannot open the file " << fileName << endl;
		return;
	}

	Parser parser;
	InstructionSet* instructionSet;
	
	instructionSet = parser.ParseAllInstructions(fp);
	int i = 0;
	while(!instructionSet->isEmpty()){
		// fetch instructions
		Instruction instruction = instructionSet->fetchInstruction();
		Instruction *inst = parser.ParseSingleInstruction(instruction);
		cout << "FUCK\n";
		if(i == 6){
			SelectInst *sinst = new SelectInst();
			sinst->isValid = true;
			sinst->tableNames.push_back("Article");
			sinst->tableNames.push_back("Comment");
			sinst->isTableNameAlias.push_back(true);
			sinst->isTableNameAlias.push_back(true);
			sinst->tableNameAlias.push_back("A");
			sinst->tableNameAlias.push_back("C");
			sinst->tableNameAliasIndex.push_back(0);
			sinst->tableNameAliasIndex.push_back(1);
			sinst->selectedAttributesNames.push_back("articleId");
			sinst->selectedAttributesNames.push_back("title");
			sinst->selectedAttributesNames.push_back("commentId");
			sinst->selectedAttributesNames.push_back("author");
			sinst->selectedAttributesNames.push_back("content");
			sinst->isSelectedAttributesTables.push_back(true);
			sinst->isSelectedAttributesTables.push_back(true);
			sinst->isSelectedAttributesTables.push_back(true);
			sinst->isSelectedAttributesTables.push_back(true);
			sinst->isSelectedAttributesTables.push_back(true);
			sinst->selectedAttributesTables.push_back("Article");
			sinst->selectedAttributesTables.push_back("A");
			sinst->selectedAttributesTables.push_back("Comment");
			sinst->selectedAttributesTables.push_back("C");
			sinst->selectedAttributesTables.push_back("C");
			sinst->selectedAttributesTablesIndex.push_back(0);
			sinst->selectedAttributesTablesIndex.push_back(1);
			sinst->selectedAttributesTablesIndex.push_back(2);
			sinst->selectedAttributesTablesIndex.push_back(3);
			sinst->selectedAttributesTablesIndex.push_back(4);
			sinst->isWHERE = false;
			sinst->isSelectAllAttrs = false;
			sinst->isCOUNT = false;
			sinst->isSUM = false;
			inst = sinst;
		}
		cout << "FUCK\n";
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
				SelectInst *sinst = dynamic_cast<SelectInst*>(inst);
				cout << "FUCK\n";
				if(tableSet.ContainTables(sinst->tableNames)){
					cout << "FUCK\n";
					if(tableSet.SelectTable(sinst)){
						Table *t = tableSet.GetSelectedTable();
						t->ShowTable();
						tableSet.DeleteSelectedTable();
						
						if(sinst->isCOUNT){
							cout << "COUNT: " << t->tuples.size() << endl;
						}
						if(sinst->isSUM){
							//t->SELECT_SumUp()
						}
					}
				}
				
				break;
			}
		}
		i++;
		cout << "FUCKYYYYYYYYYYYY111111\n";
		instructionSet->popInstruction ();
		cout << "i = " << i << endl;
	}

	fp->close ();
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


