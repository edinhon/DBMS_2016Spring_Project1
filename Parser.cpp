#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <cstring>
#include <queue>
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
	int flag = 0;
	while (getline (*inputFile, inputString, ';')) {
		flag += 1;
		Instruction* instruction = new Instruction();
		string slicedString = "\0";
		char charBuffer[1000];

		strcpy(charBuffer, inputString.c_str());
		trying = strtok (charBuffer,",("); //忽略縮排
		while (trying != NULL) { 
			//printf("%s\n", trying);
			string stringBuffer (trying);
			//printf("%s\n", trying);
			slicedString += " ";
			slicedString += stringBuffer;
			instruction->termTokens.push(stringBuffer);
	        trying = strtok (NULL, ",");
		}
		
		//cout << slicedString << endl;
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
	
	CreateInst *table;
	int type = -1;

	string inputString = instruction.getTermTokens();
	queue <string> parsing;
	char* trying;
	char charBuffer[1000];
	strcpy(charBuffer, inputString.c_str());
	trying = strtok (charBuffer," \n()"); //忽略縮排
	while (trying != NULL) {
			//string tmpt (trying);
		parsing.push((string)trying);
		trying = strtok (NULL, " \n()");
	}
	
	while (!parsing.empty()) {
		string tmpt = parsing.front();
		parsing.pop();
		if (checkStringWithoutCase(tmpt, "create")) {
			if (checkStringWithoutCase(parsing.front(), "table")) {
				//cout << "Create Table : ";
				parsing.pop();
				table = new CreateInst(parsing.front());
				parsing.pop();
				//cout << table->tableName << endl;
				type = CREATE_TABLE;
				break;
			} else {
				cout << "valid instruction on creating table\n";
				return;
			}
		}
	}
	//cout << instruction.getTermTokens() << endl;
	switch (type) {
		case CREATE_TABLE :
			bool finishOneAttribute = false;
			int currentAttribute = 0;

			while (!instruction.termTokens.empty()) {
				string tmpt = instruction.getTermTokens();
				strcpy(charBuffer, tmpt.c_str());
				trying = strtok (charBuffer," \n()"); //忽略縮排
				while (trying != NULL) {
						//string tmpt (trying);
					parsing.push((string)trying);
					trying = strtok (NULL, " \n()");
				}
				
				while (!parsing.empty()) {
					tmpt = parsing.front ();
					parsing.pop();
					if (checkStringWithoutCase(tmpt, "primary")) {
						if (checkStringWithoutCase(parsing.front(), "key")) {
							parsing.pop();
							table->isPK[currentAttribute] = true;
							finishOneAttribute = true;
							if (!parsing.empty()) { //primary key後面應該要沒東西
								cout << "primary key後面應該要沒東西\n"; //note that already pop
								return;	
							}
						} else if (parsing.empty())	{//primary 打一半  
							cout << "primary  打一半！\n";
							return;
						} else { // an attribute named primary
							table->attributeNames[currentAttribute] = tmpt;
						}
					} else if (checkStringWithoutCase(tmpt, "int")) {	//found int
						table->attributeTypes[currentAttribute] = 0;
						if (parsing.empty()) {
							finishOneAttribute = true;
						} else if (!checkStringWithoutCase(parsing.front(), "primary")) {	//int 後面有東西，非 primary key
							cout << "int 後面有東西，非 primary key!\n";
							return;
						}
					} else if (checkStringWithoutCase(tmpt, "varchar")) {
						table->attributeTypes[currentAttribute] = 1;	//found char
						if (parsing.empty()) { //varchar 後面沒有接東西
							cout << "varchar 後面沒有接東西\n";
							return;
						}
						tmpt = parsing.front();
						parsing.pop();

						int length = 0;
						for (int i=0; i<tmpt.size(); i++) {
							length *= 10;
							length += tmpt[i] - '0';
						}
						table->varCharSizes[currentAttribute] = length;
						if (parsing.empty())	//varchar 完了
							finishOneAttribute = true;
						else if (!checkStringWithoutCase(parsing.front(), "primary")) { //check後面有沒有primary key
							//if not, invalid
							cout << "varchar後面出現primary以外的東西\n";
							return;
						} 
					} else 
						table->attributeNames[currentAttribute] = tmpt;

					if (finishOneAttribute) {
						currentAttribute += 1;
						finishOneAttribute = false;
					}
				}
			}
			table->attributeNum = currentAttribute-1;
			break;
	}
	
	for (int i=0; i<10; i++) {
		cout << "attribute name " << table->attributeNames[i] << ' ' 
			<< "type " << table->attributeTypes[i] << ' '
			<< "length " << table->varCharSizes[i] << ' '
			<< "pk " << table->isPK[i] << ' '
			<< endl;
	}
	cout << "attribute numbers " << table->attributeNum << endl;
	
	/*
	switch (type) {
		case CREATE_TABLE :
			bool finishOneAttribute = false;
			int currentAttribute = 0;

			while (!instruction.termTokens.empty()) {
				finishOneAttribute = false;
				string tmpt = instruction.getTermTokens();
				if (checkStringWithoutCase(tmpt, "primary")) { //see if we are incountering primary key or an attribute
																//named primary
					if (checkStringWithoutCase(instruction.termTokens.front(), "key")) { 
						instruction.termTokens.pop ();
						table->isPK[currentAttribute] = true;
						finishOneAttribute = true;
					} else {
						table->attributeNames[currentAttribute] = tmpt;
					}
				} else if (checkStringWithoutCase(tmpt, "int")) {	//found int
					table->attributeTypes[currentAttribute] = 0;
					finishOneAttribute = true;
				} else if (checkStringWithoutCase(tmpt, "varchar")) {	//found char
					table->attributeTypes[currentAttribute] = 1;
					if (checkIfIsNumber(instruction.termTokens.front())) { 
						tmpt = instruction.getTermTokens();
						int length = 0;
						for (int i=0; i<tmpt.size(); i++) {
							length *= 10;
							length += tmpt[i] - '0';
						}
						table->varCharSizes[currentAttribute] = length;
						finishOneAttribute = true;
					} else {
						cout << "attribute type varchar without assigning length!\n";
						return;
					}
				}

				if (finishOneAttribute) {
					currentAttribute += 1;
					finishOneAttribute = false;
				}
			}
			break;
	}
*/
	
	return;
}