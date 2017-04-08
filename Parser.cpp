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
#include "SelectInst.h"


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
		
		trying = strtok (charBuffer," \n\t");

		while (trying != NULL) { 
			string stringBuffer (trying);
			
			queue<string> parse;
			int flag = 0;
			int p = stringBuffer.size();
			int j=0;
			bool catchDigit = false;
			bool dotFlag = false;

			while (j<p) {
				if ((stringBuffer[j] == '(') || (stringBuffer[j] == ')') || (stringBuffer[j] == ',') 
									|| (stringBuffer[j] == '.')) {
					if (flag == j) {

						string tmpt;
						if (stringBuffer[j] == '(')
							tmpt = "(";
						else if (stringBuffer[j] == ')')
							tmpt = ")";
						else if (stringBuffer[j] == ',')
							tmpt = ",";
						else if (stringBuffer[j] == '.')
							tmpt = ".";
						parse.push (tmpt);
						flag = j+1;
						catchDigit = false;
					} else {
						parse.push (stringBuffer.substr(flag, j-flag));
						string tmpt;
						if (stringBuffer[j] == '(')
							tmpt = "(";
						else if (stringBuffer[j] == ')')
							tmpt = ")";
						else if (stringBuffer[j] == ',')
							tmpt = ",";
						else if (stringBuffer[j] == '.') {
							tmpt = ".";
							dotFlag = true;
						}

						parse.push (tmpt);
						if (dotFlag) {
							parse.push (stringBuffer.substr(j+1, (stringBuffer.size() - j)));
						}
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

			if (catchDigit && !dotFlag) {
				instruction->setTermTokens(trying);
			}
			int t=parse.size();
			for (int r=0; r<t; r++)
			{
				instruction->setTermTokens(parse.front());
				parse.pop();
			}

	        trying = strtok (NULL, " \n\t");
		}
		instruction->setInstructionString(slicedString);
		instructionSet->pushInstruction(*instruction);
		//instruction->showTokens ();
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
	SelectInst *select;
	int type = -1;
	int tableSize = -1;

	while (!instruction.isEmpty()) {

		string thisTerm = instruction.getTermTokens();
		
		
		if (checkStringWithoutCase(thisTerm, "create")) {

			instruction.popTermTokens();
			
			if (checkStringWithoutCase(instruction.getTermTokens(), "table")) {
				instruction.popTermTokens();
				for (int i=0; i<(int)instruction.getTermTokens().size(); i++) {
					if (!isalpha(instruction.getTermTokens()[i]) && instruction.getTermTokens()[i] != '_') {
						table = new CreateInst ();
						table->isValid = false;
						cout << "- Syntax Error : table name cannot contain charactors besides alphabats or '_' " << endl;
						return table;
					}
				}
				table = new CreateInst (instruction.getTermTokens());
				instruction.popTermTokens();
				type = CREATE_TABLE;
			}
			else {
				table = new CreateInst ();
				table->isValid = false;
				cout << "- Error : wrong instruction on create table" << endl;
				return table;
			}
		} else if (checkStringWithoutCase(thisTerm, "insert")) {
			instruction.popTermTokens();
			
			if (checkStringWithoutCase(instruction.getTermTokens(), "into")) {
				instruction.popTermTokens();
				for (int i=0; i<(int)instruction.getTermTokens().size(); i++) {
					if (!isalpha(instruction.getTermTokens()[i]) && instruction.getTermTokens()[i] != '_') {
						tuple = new InsertInst ();
						tuple->isValid = false;
						cout << "- Syntax Error : table name cannot contain charactors besides alphabats or '_' " << endl;
						return tuple;
					}
				}
				tuple = new InsertInst (instruction.getTermTokens());
				instruction.popTermTokens();
				type = INSERT_TUPLE;
			}
			else {
				tuple = new InsertInst ();
				tuple->isValid = false;
				cout << "- Error :wrong instruction on insert tuple" << endl;
				return tuple;
			}
		} else if (checkStringWithoutCase(thisTerm, "select")) {
			select = new SelectInst ();
			instruction.popTermTokens();
			type = SELECTION;
		} else { }

		switch (type) {
			case CREATE_TABLE : {
				int step = 1;
				while (!instruction.isEmpty()) {
					string tmpt = instruction.getTermTokens();
					switch (step) {
						
						case 1 : {
							if (tmpt[0] == '(') {
								instruction.popTermTokens();
								step = 2;
							}
							else {
								cout << "- Syntax Error : 1" << endl;
								table->isValid = false;
								return table;
							}
							break;
						}
						case 2 : {
							if (!isalpha(tmpt[0])) {
								cout << tmpt[0] << "<--" << endl;
								cout << "- Syntax Error : not entering name" << endl;
								table->isValid = false;
								return table;
							}
							for (int i=0; i<(int)tmpt.size(); i++) {
								if (!isalpha (tmpt[i]) && tmpt[i] != '_') {
									cout << "- Syntax Error : attribute name cannot contain charactors besides alphabats or '_' " << endl;
									table->isValid = false;
									return table;
								}
							}
							table->attributeNames.push_back(tmpt);
							table->attributeTypes.push_back(-1);
							table->varCharSizes.push_back(-1);
							table->isPK.push_back(false);
							tableSize += 1;
							instruction.popTermTokens();
							step = 3;
							break;
						}
						case 3 : {
							if (checkStringWithoutCase(tmpt, "int")) {
								table->attributeTypes[tableSize] = 0;
								table->varCharSizes[tableSize] = -1;
								instruction.popTermTokens();
								step = 7;
							} else if (checkStringWithoutCase(tmpt, "varchar")) {
								table->attributeTypes[tableSize] = 1;
								instruction.popTermTokens();
								step = 4;
							} else {
								cout << "- Syntax Error : 3 type 後面格式錯誤" << endl;
								table->isValid = false;
								return table;
							}
							break;
						}
						case 4 : {
							if (tmpt[0] == '(') {
								instruction.popTermTokens();
								step = 5;
							}
							else {
								cout << "- Syntax Error : 4" << endl;
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
								cout << "- Syntax Error : varchar length cannot exceed 40" << endl;
								table->isValid = false;
								return table;
							}
							table->varCharSizes[tableSize] = length;
							instruction.popTermTokens();
							step = 6;
							break;
						}
						case 6 : {
							if (tmpt[0] == ')') {
								instruction.popTermTokens();
								step = 7;
							}
							else {
								cout << "- Syntax Error : 6" << endl;
								table->isValid = false;
								return table;
							}
							break;
						}
						case 7 : {
							if (checkStringWithoutCase(tmpt, "primary")) {
								instruction.popTermTokens();
								if (checkStringWithoutCase(instruction.getTermTokens(), "key")) {
									table->isPK[tableSize] = true;
									instruction.popTermTokens();
									step = 8;
								}
								else {
									table->isPK[tableSize] = false;
									table->isValid = false;
									cout << "- Syntax Error : 7" << endl;
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
								instruction.popTermTokens ();
								step = 2;
							}
							else if (tmpt[0] == ')') {
								instruction.popTermTokens();
							}
							else {
								table->isValid = false;
								cout << "- Syntax Error : 8, not ',' or ')'" << endl;
								return table;
							}
							break;
						}
						default : {
							table->isValid = false;
							cout << "- Error :wrong instruction on create table" << endl;
							return table;
							break;
						}
					}
				
				}
				break;
			}			
			case INSERT_TUPLE : {
				bool insertNullValue = false;
				bool catchcomma = false;
				string* attach;
				int step = 0;
				while (!instruction.isEmpty()) {
					string tmpt = instruction.getTermTokens();

					switch (step) {
						case 0 : {
							if (checkStringWithoutCase(tmpt, "values")) {
								tuple->isWithName = false;
								step = 4;
							} else {
								tuple->isWithName = true;
								step = 1;
							}
							break;
						}
						case 1 : {
							if (tmpt[0] == '(') {
								instruction.popTermTokens();
								step = 2;
							}
							else {
								tuple->isValid = false;
								cout << "- Syntax Error : 1" << endl;
								return tuple;
							}
							break;
						}
						case 2 : {
							if (isalpha(tmpt[0])) {
								for (int i=0; i<(int)tmpt.size(); i++) {
									if (!isalpha(tmpt[i]) && tmpt[i] != '_') {
										cout << "- Syntax Error : : attribute name cannot contain charactors besides alphabats or '_' " << endl;
										tuple->isValid = false;
										return tuple;
									}
								}
								tuple->insertedAttributes.push_back (tmpt);
								instruction.popTermTokens();
								step = 3;
							} else {
								tuple->isValid = false;
								cout << "- Syntax Error : 2" << endl;
								return tuple;
							}
							break;
						}
						case 3 : {
							if (tmpt[0] == ',') {
								instruction.popTermTokens();
								step = 2;
							} else if (tmpt[0] == ')') {
								instruction.popTermTokens();
								step = 4;
							}
							break;
						}
						case 4 : {
							if (checkStringWithoutCase(tmpt, "values")) {
								instruction.popTermTokens();
								step = 5;
							} else {
								tuple->isValid = false;
								cout << "- Syntax Error : 4" << endl;
								return tuple;
							}
							break;
						}
						case 5 : {
							if (tmpt[0] == '(') {
								instruction.popTermTokens();
								step = 6;
							}
							else {
								tuple->isValid = false;
								cout << "- Syntax Error : 5" << endl;
								return tuple;
							}
							break;
						}
						case 6 : {
							if (catchcomma) {
								int ts = (int)tmpt.size();
								if (tmpt[ts-1] == 39) {
									*attach += tmpt.substr(0, ts-1);
									tuple->insertedValues.push_back (attach);
									tuple->insertedValueTypes.push_back(1);
									instruction.popTermTokens();
									catchcomma = false;
									step = 7;
								} else {
									*attach += tmpt;
									*attach += " ";
									instruction.popTermTokens();
									catchcomma = true;
									step = 6;
								}
							}
							else if (tmpt[0] == 39) {
								int ts = (int)tmpt.size();
								if (tmpt[ts-1] == 39) {
									string* toAdd = new string (tmpt.substr(1, tmpt.size()-2));
									tuple->insertedValues.push_back (toAdd);
									tuple->insertedValueTypes.push_back(1);
									instruction.popTermTokens();
									catchcomma = false;
									step = 7;
								} else {
									if (!catchcomma) {
										attach = new string (tmpt.substr(1, ts-1));
										*attach += " ";
										instruction.popTermTokens();
										catchcomma = true;
										step = 6;
									} else {
										cout << "- Syntax Error : 6" << endl;
										tuple->isValid = false;
										return tuple;
									}
								}
							} else if (tmpt[0] == ',') {
								string* toAdd = new string ("NULL_VALUE");
								tuple->insertedValues.push_back (toAdd);
								tuple->insertedValueTypes.push_back(-1);
								insertNullValue = true;
								step = 7;
							} else if (tmpt[0] == ')') {
								string* toAdd = new string ("NULL_VALUE");
								tuple->insertedValues.push_back (toAdd);
								tuple->insertedValueTypes.push_back(-1);
								insertNullValue = true;
								step = 7;
							} else if (isdigit(tmpt[0])) {
								string* toAdd = new string (tmpt);
								tuple->insertedValues.push_back (toAdd);
								tuple->insertedValueTypes.push_back(0);
								instruction.popTermTokens();
								step = 7;
							}
							else {
								tuple->isValid = false;
								cout << "- Syntax Error : 6" << endl;
								return tuple;
							}
							break;
						}
						case 7 : {
							if (tmpt[0] == ',') {
								if (insertNullValue) {
									instruction.popTermTokens();
									insertNullValue = false;
									step = 6;
								} else {
									instruction.popTermTokens();
									step = 6;
								}
							} else if (tmpt[0] == ')') {
								if (insertNullValue) {
								}
								instruction.popTermTokens();
							}
							break;
						}
					}
				}
				break;
			}
			case SELECTION : {
				int step = 1;
				const int from = 2;
				const int where = 3;

				while (!instruction.isEmpty()) {
					string current = instruction.getTermTokens();

					switch (step) {
						case 1 : {
							// parsing selection targets
							if (checkStringWithoutCase(current, "from")) {
								// ready to jump to case 'from'
								// remember to parse the current string!!!!!
								cout << endl;
								instruction.popTermTokens();	// pop till 'from'
								step = from;
							} else if (current == ",") {
								// continue to do next token
								cout << ' ';
								instruction.popTermTokens ();
								step = 1;
							} else if (current == ".") {
								// selecting attributes from a certain table
								cout << ' ';
								instruction.popTermTokens ();
								step = 1;
							} else {
								// keep parsing
								cout << current;
								instruction.popTermTokens ();
								step = 1;
							}

							break;
						}
						case from : {
							cout << "In case \'from\' " << endl;
							if (checkStringWithoutCase(current, "where")) {
								// jump to where
								cout << endl;
								instruction.popTermTokens();	// pop till 'where'
								step = where;
							} else if (checkStringWithoutCase (current, "and")) {
								// 'and' instruction
								cout << ' ';
								instruction.popTermTokens ();
								step = from;
							} else if (checkStringWithoutCase (current, "or")) {
								// 'or' instruction
								cout << ' ';
								instruction.popTermTokens ();
								step = from;
							} else if (checkStringWithoutCase (current, "as")) {
								// 'as' instruction
								cout <<  ' ';
								instruction.popTermTokens ();
								step = from;
							} else {
								// keep parsing
								cout << current;
								instruction.popTermTokens ();	
								step = from;
							}
							break;
						}
						case where : {
							cout << "In case \'where\'" << endl;
							if (checkStringWithoutCase (current, "and")) {
								// 'and' instruction
								cout << ' ';
								instruction.popTermTokens ();
								step = from;
							} else if (checkStringWithoutCase (current, "or")) {
								// 'or' instruction
								cout << ' ';
								instruction.popTermTokens ();
								step = from;
							} else {
								// parsing
								instruction.popTermTokens ();
								step = where;
							}
							break;
						}
						default : {
							cout << "Into default" << endl;
							break;
						}
					}
				}
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
			table->isValid = true;
			return table;
			break;
		}
		case INSERT_TUPLE : {
			
			if ((tuple->insertedAttributes.size() != 0) && (tuple->insertedAttributes.size() != tuple->insertedValues.size())) {
				tuple->isValid = false;
				return tuple;
				break;
			}
			tuple->isValid = true;
			return tuple;
			break;
		}
		case SELECTION : {
			select->isValid = true;
			return select;
			break;
		}
		default : {
			Instruction* nullinst = new Instruction();
			cout << "- Syntax Error : an error occurred that causes return null instruction" << endl;
			return nullinst;
			break;
		}
	}
}