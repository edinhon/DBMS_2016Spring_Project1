#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
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
	//cout << "* parsing into tokens" << endl;
	InstructionSet* instructionSet = new InstructionSet ();
	string inputString;

	while (getline (*inputFile, inputString, ';')) {
		
		//cout << "> " << inputString << endl;
		Instruction* instruction = new Instruction();
		string slicedString = "\0";
		char charBuffer[1000];
		char* trying;

		strcpy(charBuffer, inputString.c_str());
		
		trying = strtok (charBuffer," \n"); //忽略縮排

		while (trying != NULL) { 
			string stringBuffer (trying);
			//cout << "* "<<stringBuffer << endl;
			queue<string> parse;
			int flag = 0;
			int p = stringBuffer.size();
			int j=0;
			bool catchDigit = false;

			while (j<p) {
				if ((stringBuffer[j] == '(') || (stringBuffer[j] == ')') || (stringBuffer[j] == ',')) {
					if (flag == j) {
						//cout << "	catch one : " << stringBuffer[flag] << endl;
						string ttt;
						if (stringBuffer[j] == '(')
							ttt = "(";
						else if (stringBuffer[j] == ')')
							ttt = ")";
						else if (stringBuffer[j] == ',')
							ttt = ",";
						parse.push (ttt);
						flag = j+1;
						catchDigit = false;
					} else {
						//cout << "	catch stirng : ";
						//cout << stringBuffer.substr(flag, j-flag) << "   " << stringBuffer[j]<< endl;
						parse.push (stringBuffer.substr(flag, j-flag));

						string ttt;
						if (stringBuffer[j] == '(')
							ttt = "(";
						else if (stringBuffer[j] == ')')
							ttt = ")";
						else if (stringBuffer[j] == ',')
							ttt = ",";

						parse.push (ttt);

						flag = j+1;
						catchDigit = false;
					}
				}
				else {
					if (!catchDigit) {
						flag = j;
						catchDigit = true;
					}
				}

				j++;
			}
			if (catchDigit) {
				instruction->termTokens.push(trying);
			}
			int t=parse.size();
			for (int r=0; r<t; r++)
			{
				//cout << "- " << parse.front() << endl;
				instruction->termTokens.push(parse.front());
				parse.pop();
			}
	        trying = strtok (NULL, " \n");
		}

		/*
		int k = instruction->termTokens.size();
		for (int i=0; i<k; i++)
		{
			cout << instruction->termTokens.front() << endl;
			instruction->termTokens.pop();
		}
		*/
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
	CreateInst *table;
	InsertInst *tuple;
	int type = -1;
	int tableSize = -1;

	while (!instruction.termTokens.empty()) {
		//cout << "> slicing tokens : " << instruction.termTokens.front() << endl;

		string thisTerm = instruction.getTermTokens();
		
		
		if (checkStringWithoutCase(thisTerm, "create")) {

			instruction.termTokens.pop();
			//cout << instruction.termTokens.front() << instruction.termTokens.front() << endl;
			
			if (checkStringWithoutCase(instruction.termTokens.front(), "table")) {
				instruction.termTokens.pop();
				for (int i=0; i<(int)instruction.termTokens.front().size(); i++) {
					if (!isalpha(instruction.termTokens.front()[i]) && instruction.termTokens.front()[i] != '_') {
						table = new CreateInst ();
						table->isValid = false;
						cout << "table name cannot contain charactors besides alphabats or '_' " << endl;
						return table;
					}
				}
				//cout << "create table : " << instruction.termTokens.front() << endl;
				table = new CreateInst (instruction.termTokens.front());
				instruction.termTokens.pop();
				type = CREATE_TABLE;
			}
			else {
				table = new CreateInst ();
				table->isValid = false;
				cout << "wrong instruction on create table" << endl;
				return table;
			}
		} else if (checkStringWithoutCase(thisTerm, "insert")) {
			instruction.termTokens.pop();
			//cout << parsing.front() << parsing.front().size() << endl;
			
			if (checkStringWithoutCase(instruction.termTokens.front(), "into")) {
				instruction.termTokens.pop();
				for (int i=0; i<(int)instruction.termTokens.front().size(); i++) {
					if (!isalpha(instruction.termTokens.front()[i]) && instruction.termTokens.front()[i] != '_') {
						tuple = new InsertInst ();
						tuple->isValid = false;
						cout << "syntax error : table name cannot contain charactors besides alphabats or '_' " << endl;
						return tuple;
					}
				}
				//cout << "insert tuple into  : " << instruction.termTokens.front() << endl;
				tuple = new InsertInst (instruction.termTokens.front());
				instruction.termTokens.pop();
				type = INSERT_TUPLE;
			}
			else {
				tuple = new InsertInst ();
				tuple->isValid = false;
				cout << "wrong instruction on insert tuple" << endl;
				return tuple;
			}
		} else {
			//cout << "not create or insert" << endl;
		}

		switch (type) {
			case CREATE_TABLE : {
				//cout << ">>>	creating table " << endl;
				int step = 1;
				while (!instruction.termTokens.empty()) {
					string tmpt = instruction.termTokens.front();
					//cout << step << ' ' << tmpt << endl;
					switch (step) {
						
						case 1 : {
							if (tmpt[0] == '(') {
								instruction.termTokens.pop();
								step = 2;
							}
							else {
								cout << "syntax error 1" << endl;
								table->isValid = false;
								return table;
							}
							break;
						}
						case 2 : {
							if (!isalpha(tmpt[0])) {
								cout << "syntax error : not entering name" << endl;
								table->isValid = false;
								return table;
							}
							for (int i=0; i<(int)tmpt.size(); i++) {
								if (!isalpha (tmpt[i]) && tmpt[i] != '_') {
									cout << "syntax error : attribute name cannot contain charactors besides alphabats or '_' " << endl;
									table->isValid = false;
									return table;
								}
							}
							table->attributeNames.push_back(tmpt);
							table->attributeTypes.push_back(-1);
							table->varCharSizes.push_back(-1);
							table->isPK.push_back(false);
							tableSize += 1;
							instruction.termTokens.pop();
							step = 3;
							break;
						}
						case 3 : {
							if (checkStringWithoutCase(tmpt, "int")) {
								table->attributeTypes[tableSize] = 0;
								table->varCharSizes[tableSize] = -1;
								instruction.termTokens.pop();
								step = 7;
							} else if (checkStringWithoutCase(tmpt, "varchar")) {
								table->attributeTypes[tableSize] = 1;
								instruction.termTokens.pop();
								step = 4;
							} else {
								cout << "syntax error : type 後面格式錯誤" << endl;
							}
							break;
						}
						case 4 : {
							if (tmpt[0] == '(') {
								instruction.termTokens.pop();
								step = 5;
							}
							else {
								cout << "syntax error 4" << endl;
								table->isValid = false;
								return table;
							}
							break;
						}
						case 5 : {
							int length = 0;
							for (int i=0; i<(int)tmpt.size(); i++) {
								length *= 10;
								length += tmpt[i] - '0';
							}
							if (length > 40) {
								cout << "syntax error : varchar length cannot exceed 40" << endl;
								table->isValid = false;
								return table;
							}
							table->varCharSizes[tableSize] = length;
							instruction.termTokens.pop();
							step = 6;
							break;
						}
						case 6 : {
							if (tmpt[0] == ')') {
								instruction.termTokens.pop();
								step = 7;
							}
							else {
								cout << "syntax error 6" << endl;
								table->isValid = false;
								return table;
							}
							break;
						}
						case 7 : {
							if (checkStringWithoutCase(tmpt, "primary")) {
								instruction.termTokens.pop();
								if (checkStringWithoutCase(instruction.termTokens.front(), "key")) {
									//cout << "set primary key " << tableSize <<endl;
									table->isPK[tableSize] = true;
									instruction.termTokens.pop();
									step = 8;
								}
								else {
									table->isPK[tableSize] = false;
									table->isValid = false;
									cout << "syntax error 7" << endl;
									return table;
								}
							} else {
								table->isPK[tableSize] = false;
								step = 8;
							}
							break;
						}
						case 8 : {
							if (tmpt[0] == ',') {
								instruction.termTokens.pop ();
								step = 2;
							}
							else if (tmpt[0] == ')') {
								//cout << "end of create table switches" << endl;
								instruction.termTokens.pop();
							}
							else {
								table->isValid = false;
								cout << "syntax error 8, not ',' or ')'" << endl;
								return table;
							}
							break;
						}
						default : {
							table->isValid = false;
							cout << "wrong instruction on create table" << endl;
							return table;
							break;
						}
					}
				
				}
				break;
			}
			//--------
			
			case INSERT_TUPLE : {
				bool insertNullValue = false;
				//cout << ">>>	inserting tuple :: " << endl;
				int step = 0;
				while (!instruction.termTokens.empty()) {
					string tmpt = instruction.termTokens.front();
					//cout << step << ' ' << tmpt << endl;

					switch (step) {
						case 0 : {
							if (checkStringWithoutCase(tmpt, "values")) {
								//cout << "insert without names" << endl;
								tuple->isWithName = false;
								//instruction.termTokens.pop();
								step = 4;
							} else {
								//cout << "start assigning names" << endl;
								tuple->isWithName = true;
								step = 1;
							}
							break;
						}
						case 1 : {
							if (tmpt[0] == '(') {
								instruction.termTokens.pop();
								step = 2;
							}
							else {
								tuple->isValid = false;
								cout << "syntax error 1" << endl;
								return tuple;
							}
							break;
						}
						case 2 : {
							if (isalpha(tmpt[0])) {
								for (int i=0; i<(int)tmpt.size(); i++) {
									if (!isalpha(tmpt[i]) && tmpt[i] != '_') {
										cout << "syntax error : attribute name cannot contain charactors besides alphabats or '_' " << endl;
										tuple->isValid = false;
										return tuple;
									}
								}
								tuple->insertedAttributes.push_back (tmpt);
								instruction.termTokens.pop();
								step = 3;
							} else {
								tuple->isValid = false;
								cout << "syntax error 2" << endl;
								return tuple;
							}
							break;
						}
						case 3 : {
							if (tmpt[0] == ',') {
								instruction.termTokens.pop();
								step = 2;
							} else if (tmpt[0] == ')') {
								//cout << "end insertint attributes names" << endl;
								instruction.termTokens.pop();
								step = 4;
							}
							break;
						}
						case 4 : {
							if (checkStringWithoutCase(tmpt, "values")) {
								//cout << "start assigning values" << endl;
								instruction.termTokens.pop();
								step = 5;
							} else {
								tuple->isValid = false;
								cout << "syntax error 4" << endl;
								return tuple;
							}
							break;
						}
						case 5 : {
							if (tmpt[0] == '(') {
								instruction.termTokens.pop();
								step = 6;
							}
							else {
								tuple->isValid = false;
								cout << "syntax error 5" << endl;
								return tuple;
							}
							break;
						}
						case 6 : {
							if (tmpt[0] == 39) {
								string* toAdd = new string (tmpt.substr(1, tmpt.size()-2));
								tuple->insertedValues.push_back (toAdd);
								tuple->insertedValueTypes.push_back(1);
								instruction.termTokens.pop();
								//cout << "inserting string " << *toAdd << endl;
								step = 7;
							} else if (tmpt[0] == ',') {
								//cout << "insert null value" << endl;
								string* toAdd = new string ("NULL_VALUE");
								tuple->insertedValues.push_back (toAdd);
								tuple->insertedValueTypes.push_back(-1);
								insertNullValue = true;
								step = 7;
							} else if (tmpt[0] == ')') {
								//cout << "ending with a null value" << endl;
								string* toAdd = new string ("NULL_VALUE");
								tuple->insertedValues.push_back (toAdd);
								tuple->insertedValueTypes.push_back(-1);
								insertNullValue = true;
								step = 7;
							} else if (isdigit(tmpt[0])) {
								//cout << "inserting digit" << endl;
								string* toAdd = new string (tmpt);
								tuple->insertedValues.push_back (toAdd);
								tuple->insertedValueTypes.push_back(0);
								instruction.termTokens.pop();
								step = 7;
							}
							else {
								tuple->isValid = false;
								cout << "syntax error 6" << endl;
								return tuple;
							}
							break;
						}
						case 7 : {
							if (tmpt[0] == ',') {
								if (insertNullValue) {
									instruction.termTokens.pop();
									insertNullValue = false;
									step = 6;
								} else {
									instruction.termTokens.pop();
									step = 6;
								}
							} else if (tmpt[0] == ')') {
								if (insertNullValue) {
									//cout << "--ending with a null value" << endl;
								}
								//cout << "end insertint value names" << endl;
								instruction.termTokens.pop();
							}
							break;
						}
					}
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
			
			if ((tuple->insertedAttributes.size() != 0) && (tuple->insertedAttributes.size() != tuple->insertedValues.size())) {
				//cout << "attribute size != value size" << endl;
				tuple->isValid = false;
				return tuple;
				break;
			}
			/*
			cout << tuple->insertedAttributes.size() << ' ' << tuple->insertedValues.size() << ' ' << tuple->insertedValueTypes.size() << endl;
			
			if (tuple->insertedAttributes.size() != 0)
				for (int i=0; i<(int)tuple->insertedAttributes.size(); i++) {
					cout << tuple->insertedAttributes[i] << ' ' << *tuple->insertedValues[i] << ' ' << tuple->insertedValueTypes[i] << endl;
				}
			else {
				for (int i=0; i<(int)tuple->insertedValues.size(); i++) {
					cout << "no attribute name" << ' ' << *tuple->insertedValues[i] << ' ' << tuple->insertedValueTypes[i] << endl;
				}
			}
			*/
			tuple->isValid = true;
			//cout << "- end parsing INSERT" << endl;
			return tuple;
			break;
		}
		default : {
			Instruction* nullinst = new Instruction();
			cout << "an error occurred that causes return null instruction" << endl;
			//cout << "- end parsing" << endl;
			return nullinst;
			break;
		}
	}
}