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
	while (getline (*inputFile, inputString, ';')) {
		//cout << inputString << endl;
		//cout << "size = " << inputString.size() << endl;
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
// Instruction* ParseSingleInstruction(Instruction)
//		Parse single instruction into instruction terms set.
//---------------------------
Instruction* Parser::ParseSingleInstruction(Instruction instruction)
{
	//TODO: Decide which type of this instruction, 
	//	and new a corresponding instruction class, 
	//	and push it into instructionSet.
	
	CreateInst *table;
	InsertInst *tuple;
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
				cout << "Create Table : ";
				parsing.pop();
				table = new CreateInst(parsing.front());
				parsing.pop();
				cout << table->tableName << endl;
				type = CREATE_TABLE;
				break;
			} else {
				cout << "valid instruction on creating table\n";
				Instruction *nullInst = new Instruction();
				nullInst->isValid = false;
				return nullInst;
			}
		} else if (checkStringWithoutCase(tmpt, "insert")) {
			if (checkStringWithoutCase(parsing.front(), "into")) {
				cout << "Insert Table : ";
				parsing.pop();
				tuple = new InsertInst(parsing.front());
				parsing.pop();
				cout << tuple->tableName << endl;
				type = INSERT_TUPLE;
				break;
			} else {
				cout << "valid instruction on inserting table\n";
				Instruction *nullInst = new Instruction();
				nullInst->isValid = false;
				return nullInst;
			}
		}
	}
	//cout << instruction.getTermTokens() << endl;
	switch (type) {
		case CREATE_TABLE :{
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
								table->isValid = false;
								return table;
							}
						} else if (parsing.empty())	{//primary 打一半  
							cout << "primary  打一半！\n";
							table->isValid = false;
							return table;
						} else { // an attribute named primary
							table->attributeNames[currentAttribute] = tmpt;
						}
					} else if (checkStringWithoutCase(tmpt, "int")) {	//found int
						table->attributeTypes[currentAttribute] = 0;
						if (parsing.empty()) {
							finishOneAttribute = true;
						} else if (!checkStringWithoutCase(parsing.front(), "primary")) {	//int 後面有東西，非 primary key
							cout << "int 後面有東西，非 primary key!\n";
							table->isValid = false;
							return table;
						}
					} else if (checkStringWithoutCase(tmpt, "varchar")) {
						table->attributeTypes[currentAttribute] = 1;	//found char
						if (parsing.empty()) { //varchar 後面沒有接東西
							cout << "varchar 後面沒有接東西\n";
							table->isValid = false;
							return table;
						}
						tmpt = parsing.front();
						parsing.pop();

						int length = 0;
						for (int i=0; i< (int)tmpt.size(); i++) {
							length *= 10;
							length += tmpt[i] - '0';
						}
						if (length > 40) {
							cout << "varchar length > 40!\n";
							table->isValid = false;
							return table;
						}
						table->varCharSizes[currentAttribute] = length;
						if (parsing.empty())	//varchar 完了
							finishOneAttribute = true;
						else if (!checkStringWithoutCase(parsing.front(), "primary")) { //check後面有沒有primary key
							//if not, invalid
							cout << "varchar後面出現primary以外的東西\n";
							table->isValid = false;
							return table;
							//return nullptr;
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
			return table;
			break;
		}
		case INSERT_TUPLE : {
			queue <string> attributeNames;
			queue <string> attributeValues;
			int parsingOrder=0;

			while (!instruction.termTokens.empty()) {
				string tmpt = instruction.getTermTokens();
				strcpy(charBuffer, tmpt.c_str());
				trying = strtok (charBuffer," \n()"); //忽略縮排
				while (trying != NULL) {

					if (checkStringWithoutCase(parsing.back(), "values")) {
							parsingOrder = 1;
					} else if (checkStringWithoutCase((string)trying, "values")) {// 完整輸入 attributes 後的 value
						//cout << "changed !! " << trying << endl;
						parsingOrder = 1;
						trying = strtok (NULL, " \n()");
						continue;
					}

					switch (parsingOrder) {
						case 0: 
							attributeNames.push((string)trying);
							//printf("name : %s\n", trying);
							break;
						case 1: 
							attributeValues.push((string)trying);
							//printf("value : %s\n", trying);
							break;
						default : 
							cout << "why 跑到 default 呢呢呢呢呢\n";
							break;
					}
					parsing.push((string)trying);
					trying = strtok (NULL, " \n()");
				}
			}
			if (attributeNames.size() > 0 && (attributeNames.size() != attributeValues.size())) {
				// not necessary to input attribute names
				//cout << attributeNames.size() << ' ' << attributeValues.size() << endl;
				cout << "name size != value size" << endl;
				tuple->isValid = false;
				return tuple; 
			}
			tuple->isValid = true;
			return tuple;
			break;
		}
		default:
			Instruction *nullInst = new Instruction();
			nullInst->isValid = false;
			return nullInst;
			break;
	}
	/*
	for (int i=0; i<10; i++) {
		cout << "attribute name " << table->attributeNames[i] << ' ' 
			<< "type " << table->attributeTypes[i] << ' '
			<< "length " << table->varCharSizes[i] << ' '
			<< "pk " << table->isPK[i] << ' '
			<< endl;
	}
	cout << "attribute numbers " << table->attributeNum << endl;
	*/
	return nullptr;
}