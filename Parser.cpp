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

	while (getline (*inputFile, inputString, ';')) {
		Instruction* instruction = new Instruction();
		string slicedString = "\0";
		char charBuffer[1000];
		char* trying;

		strcpy(charBuffer, inputString.c_str());
		trying = strtok (charBuffer,","); //忽略縮排
		while (trying != NULL) { 
			string stringBuffer (trying);
			
			slicedString += stringBuffer;
			instruction->setTermTokens(stringBuffer);
	        trying = strtok (NULL, ",");
		}
		instruction->setInstructionString(slicedString);
		instructionSet->pushInstruction(*instruction);
	}
	//cout << "End parsing input case\n#######\n#######" << endl;
	return instructionSet;
}

//---------------------------
// Instruction* ParseSingleInstruction(Instruction)
//		Parse single instruction into instruction terms set.
//---------------------------
Instruction* Parser::ParseSingleInstruction(Instruction instruction)
{
	CreateInst *table;
	InsertInst *tuple;
	int type = -1;
	int tableSize = -1;
	bool flag = false;

	while (!instruction.termTokens.empty()) {
		string thisTerm = instruction.getTermTokens();
		//cout << "###"<<thisTerm << endl;
		instruction.popTermTokens();
		
		queue <string> parsing;
		char* trying;
		char charBuffer[1000];
		strcpy(charBuffer, thisTerm.c_str());
		trying = strtok (charBuffer," \n()"); //忽略縮排
		if (trying == NULL) {
			Instruction* nullinst = new Instruction();
			return nullinst;
		}
		while (trying != NULL) {
			parsing.push((string)trying);
			//printf("*  %s\n", trying);
			trying = strtok (NULL, " \n()");
		}
		if (checkStringWithoutCase(parsing.front(), "create")) {
			parsing.pop();
			//cout << parsing.front() << parsing.front().size() << endl;
			
			if (checkStringWithoutCase(parsing.front(), "table")) {
				parsing.pop();
				//cout << "create table : " << parsing.front() << endl;
				table = new CreateInst (parsing.front());
				parsing.pop();
				type = CREATE_TABLE;
			}
			else {
				table = new CreateInst ();
				table->isValid = false;
				cout << "wrong instruction on create table : perhaps spelling" << endl;
				return table;
			}
		} else if (checkStringWithoutCase(parsing.front(), "insert")) {
			parsing.pop();
			//cout << parsing.front() << parsing.front().size() << endl;
			
			if (checkStringWithoutCase(parsing.front(), "into")) {
				parsing.pop();
				//cout << "insert tuple into  : " << parsing.front() << endl;
				tuple = new InsertInst (parsing.front());
				parsing.pop();
				type = INSERT_TUPLE;
			}
			else {
				tuple = new InsertInst ();
				tuple->isValid = false;
				cout << "wrong instruction on insert tuple : perhaps spelling" << endl;
				return tuple;
			}
		} else {
			//cout << "not create or insert" << endl;
		}

		switch (type) {
			case CREATE_TABLE : {
				//cout << "creating table " << endl;

				int step = 1;
				while (!parsing.empty()) {
					string tmpt = parsing.front();
					//cout << step << ' ' << tmpt << endl;
					
					switch (step) {
						case 1 : {
							table->attributeNames.push_back(tmpt);
							table->attributeTypes.push_back(-1);
							table->varCharSizes.push_back(-1);
							table->isPK.push_back(false);
							tableSize += 1;
							parsing.pop();
							step = 2;
							break;
						}
						case 2 : {
							if (checkStringWithoutCase(tmpt, "int")) {
								table->attributeTypes[tableSize] = 0;
								table->varCharSizes[tableSize] = -1;
								parsing.pop();
								step = 4;
							} else if (checkStringWithoutCase(tmpt, "varchar")) {
								table->attributeTypes[tableSize] = 1;
								parsing.pop();
								step = 3;
							} else {
								cout << "type 後面格式錯誤" << endl;
							}
							break;
						}
						case 3 : {
							int length = 0;
							for (int i=0; i<(int)tmpt.size(); i++) {
								length *= 10;
								length += tmpt[i] - '0';
							}
							table->varCharSizes[tableSize] = length;
							parsing.pop();
							step = 4;
							break;
						}
						case 4 : {
							if (checkStringWithoutCase(tmpt, "primary")) {
								parsing.pop();
								if (checkStringWithoutCase(parsing.front(), "key")) {
									//cout << "set primary key " << tableSize <<endl;
									table->isPK[tableSize] = true;
									parsing.pop();
									step = 1;
								}
								else {
									table->isPK[tableSize] = false;
									//step = 1;
								}
							} else {
								table->isPK[tableSize] = false;
							}

							step = 1;
							break;
						}
						default : {
							table->isValid = false;
							cout << "wrong instruction on create table : perhaps spelling" << endl;
							return table;
							break;
						}
					}
				
				}
				break;
			}
			//--------
			case INSERT_TUPLE : {
				//cout << "inserting tuple" << endl;
				while (!parsing.empty()) {
					string tmpt = parsing.front();
					//cout << tmpt << endl;
					if (checkStringWithoutCase(tmpt, "values")) {
							flag = true;
							parsing.pop();
							continue;
					}

					if (!flag) {
						tuple->insertedAttributes.push_back(tmpt);
					} else {
						if (tmpt[0] == 39) {
							string* tmpt1 = new string (tmpt.substr(1, tmpt.size()-2));
							tuple->insertedValueTypes.push_back(1);
							tuple->insertedValues.push_back(tmpt1);
						} else {
							string *tmpt1 = new string (tmpt);
							tuple->insertedValueTypes.push_back(0);
							tuple->insertedValues.push_back(tmpt1);
						}
					}

					parsing.pop();
				}
				break;
			}
			default : {
				Instruction* nullinst = new Instruction();
				return nullinst;
				break;
			}
		}
		
	}

	switch (type) {
		case CREATE_TABLE : {
			/*
			cout << table->attributeNames.size() << ' ' << table->attributeTypes.size() << ' ' << table->varCharSizes.size() << ' ' << table->isPK.size() << endl;
			
			for (int i=0; i<table->varCharSizes.size(); i++)
				cout << table->attributeNames[i] << ' ' << table->attributeTypes[i] << ' ' << table->varCharSizes[i] << ' ' << table->isPK[i] << endl;
			*/
			table->isValid = true;
			return table;
			break;
		}
		case INSERT_TUPLE : {
			/*
			cout << tuple->insertedAttributes.size() << ' ' << tuple->insertedValues.size() << ' ' << tuple->insertedValueTypes.size() << endl;
			for (int i=0; i<tuple->insertedAttributes.size(); i++)
				cout << tuple->insertedAttributes[i] << ' ' << tuple->insertedValues[i] << ' ' << tuple->insertedValueTypes[i] << endl;
			*/
			tuple->isValid = true;
			return tuple;
			break;
		}
		default : {
			Instruction* nullinst = new Instruction();
			return nullinst;
			break;
		}
	}
}