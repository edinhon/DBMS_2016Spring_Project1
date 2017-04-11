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
			bool alreadyPushed = false;
			while (j < p) {
				if (!isalpha(stringBuffer[j])) {
					if (!isdigit(stringBuffer[j])) {
						if (flag == j) {
							parse.push (stringBuffer.substr(j, 1));
							alreadyPushed = true;
							flag = j+1;
						}
						else {
							parse.push (stringBuffer.substr(flag, j-flag));
							parse.push (stringBuffer.substr(j, 1));
							alreadyPushed = true;
							flag = j+1;
						}
					} else {
						if (alreadyPushed)
							flag = j;
						alreadyPushed = false;
					}
				} else {
					alreadyPushed = false;
				}
				j++;
			}
			if (!alreadyPushed) {
				parse.push (stringBuffer.substr (flag, p-flag));
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
	bool catchingNot = false;
	vector <string> selectedTableLeft, left, operation, selectedTableRight, right;
	vector <int> selectLeftType, selectRightType;
	vector <string> startTableNames, startAttributeNames;
	vector <string> fromTableNames, fromTableShorthands;
	bool withExpression = false;


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
		} else { 
			Instruction* nullinst = new Instruction();
			cout << "I don't know what can lead us here" << endl;
			return nullinst;
		}

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
							if (tmpt == "'" || catchcomma) {
								if (catchcomma) {
									if (tmpt == "'") {
										tuple->insertedValues.push_back (attach);
										tuple->insertedValueTypes.push_back(1);
										instruction.popTermTokens();
										catchcomma = false;
										step = 7;
									} else {
										if (*attach != "")
											*attach += " ";
										*attach += tmpt;
										instruction.popTermTokens ();
										catchcomma = true;
										step = 6;
									}
								} else {
									attach = new string ();
									instruction.popTermTokens ();
									catchcomma = true;
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
				const int start = 1;
				const int from = 2;
				const int where = 3;

				// in 
				bool selectingInParticularTable = false;	// in and where
				bool selectCount = false, selectSum = false;
				bool selectLeftParenthesis = false;
				bool endOfParenthesis = false;
				//int fromStep = 0;
				// in case from :
				int caughtAlias = 0;
				// in case where :
				bool assigning = false;	/*
											in where, 
											if assigning == false 
												we are on the left side of the assignment
											else 
												we are on the right side of the assignment
										*/
				bool catchcomma = false;
				string* attach;

				string last;
				int step = start;
				int fromStep = 0;
				while (!instruction.isEmpty()) {
					string current = instruction.getTermTokens();

					switch (step) {
						case start : {

							if (endOfParenthesis) {
								if (!checkStringWithoutCase (current, "from")) {
									cout << "end of parenthesis and hasn't finish select" << endl;
									select->isValid = false;
									return select;
								}
								endOfParenthesis = false;
								step = from;
							}
							// parsing selection targets
							if (checkStringWithoutCase(current, "from")) {
								// ready to jump to case 'from'
								// remember to parse the current string!!!!!
								instruction.popTermTokens();	// pop till 'from'
								step = from;
							} else if (checkStringWithoutCase (current, "count")) {
								// count (attributes)
								select->isCOUNT = true;
								selectCount = true;
								instruction.popTermTokens ();
								step = start;
							} else if (checkStringWithoutCase (current, "sum")) {
								// count (attributes)
								select->isSUM = true;
								selectSum = true;
								instruction.popTermTokens ();
								step = start;
							} else if (current == "(") {
								// left parenthesis of count
								if (!selectCount && !selectSum) {
									cout << "Syntax Error : in selecting count" << endl;
									select->isValid = false;
									return select;
								} else {
									selectLeftParenthesis = true;
									instruction.popTermTokens ();
									step = start;
								}
							} else if (current == ")") {
								// left parenthesis of count
								if ((!selectCount && !selectSum) || !selectLeftParenthesis) {
									cout << "Syntax Error : in selecting count" << endl;
									select->isValid = false;
									return select;
								} else {
									selectLeftParenthesis = false;
									selectCount = false;
									selectSum = false;
									endOfParenthesis = true;
									instruction.popTermTokens ();
									step = start;
								}
							} else if (current == ",") {
								// continue to do next token
								instruction.popTermTokens ();
								step = start;
							} else if (current == ".") {
								// selecting attributes from a certain table
								selectingInParticularTable = true;
								instruction.popTermTokens ();
								step = start;

								// continue so the assign-to-last won't execute
								//continue;
							} else {
								// keep parsing
								// selectedAttributesNames //
								if (selectingInParticularTable) {
									if (current == "*") {
										startTableNames.pop_back ();
										startTableNames.push_back (last);
										startAttributeNames.pop_back ();
										startAttributeNames.push_back ("*");
										selectingInParticularTable = false;
									} else {
										// in special cases, when we assign tables
										startTableNames.pop_back ();
										startTableNames.push_back (last);
										startAttributeNames.pop_back ();
										startAttributeNames.push_back (current);
										selectingInParticularTable = false;
									}
								} else {
									if (current == "*") {
										startTableNames.push_back ("");
										startAttributeNames.push_back ("*");
									} else {
										// in nromal cases, push back attribute to target
										startTableNames.push_back ("");
										startAttributeNames.push_back (current);
									}
								}
								last = current;
								instruction.popTermTokens ();
								step = start;
							}
							break;
						}
						case from : {	
							switch (fromStep) {
								case 0: {
									// table name
									if (caughtAlias == 4) {
										//沒經過逗號就回來了
										cout << "Syntax Error : line 599" << endl;
										select->isValid = false;
										return select;
									}

									if (checkStringWithoutCase (current, "where")) {
										if (fromTableNames.size () == 0) {
											cout << "Syntax Error : line 601" << endl;
											select->isValid = false;
											return select;
										} 
										instruction.popTermTokens ();
										step = where;
										select->isWHERE = true;
									} else {
										caughtAlias = 1;
										fromTableNames.push_back (current);
										fromTableShorthands.push_back ("");
										step = from;
										fromStep = 1;
										instruction.popTermTokens ();
									}
									break;
								}
								case 1: {
									if (checkStringWithoutCase (current, ",")) {
										caughtAlias = 3;
										instruction.popTermTokens ();
										step = from;
										fromStep = 0;
									} else if (checkStringWithoutCase (current, "as")) {
										if (caughtAlias == 4) {
											//沒經過逗號就回來了
											cout << "Syntax Error : line 633" << endl;
											select->isValid = false;
											return select;
										}
										fromTableShorthands.pop_back ();
										instruction.popTermTokens ();
										current = instruction.getTermTokens ();
										fromTableShorthands.push_back (current);	// found alias
										instruction.popTermTokens();
										step = from;
										cout << "after as : " << instruction.getTermTokens () << endl;
										fromStep = 1;
										caughtAlias = 4;
									} else if (checkStringWithoutCase (current, "where")) {
										// 
										//cout << "here" << fromTableNames.size() << ' ' << fromTableShorthands.size () << endl;
										select->isWHERE = true;
										instruction.popTermTokens ();
										step = where;
										//caughtAlias = 4;
									} else {
										// straight to alias
										if (caughtAlias == 4) {
											//沒經過逗號就回來了
											cout << "Syntax Error : line 657" << endl;
											select->isValid = false;
											return select;
										}
										fromTableShorthands.pop_back ();
										fromTableShorthands.push_back (current);
										instruction.popTermTokens ();
										step = from;
										fromStep = 1;
										caughtAlias = 4;
									}
									break;
								}
							}
							break;
						}
						case where : {
							//cout << "Caught \'where\' instruction!" << endl;
							if (checkStringWithoutCase (current, "AND")) {
								// 'and' instruction
								if (!withExpression) {
									cout << "and without expression!!!!!!!!!!!!!!!!" << endl;

									selectedTableRight.push_back ("");
									right.push_back ("");
									selectRightType.push_back (-1);
									operation.push_back ("NULL");
								}
								withExpression = false;

								select->WHERE_ExprAO = 0;
								instruction.popTermTokens ();
								assigning = false;
								step = where;
							} else if (checkStringWithoutCase (current, "OR")) {
								// 'or' instruction
								select->WHERE_ExprAO = 1;
								if (!withExpression) {
									cout << "or without expression!!!!!!!!!!!!!!!!" << endl;
									
									selectedTableRight.push_back ("");
									right.push_back ("");
									selectRightType.push_back (-1);
									operation.push_back ("NULL");
								}
								withExpression = false;
								instruction.popTermTokens ();
								assigning = false;
								step = where;

							} else if (current == "=") {
								withExpression = true;
								assigning = true;
								operation.push_back (current);
								instruction.popTermTokens ();
								step = where;
							} else if (current == ">") {
								withExpression = true;
								if (catchingNot) {
									operation.pop_back ();
									operation.push_back ("<>");
									catchingNot = false;
								} else 
									operation.push_back (current);
								assigning = true;
								//operation.push_back (current);
								instruction.popTermTokens ();
								step = where;
							} else if (current == "<") {
								withExpression = true;
								catchingNot = true;
								assigning = true;
								operation.push_back (current);
								instruction.popTermTokens ();
								step = where;
							} else if (current == ".") {
								// on certain table
								// type would be attribute
								if (!assigning) {
									left.pop_back ();
									selectLeftType.pop_back ();
									selectedTableLeft.pop_back ();
									selectedTableLeft.push_back (last);
									selectingInParticularTable = true;
								} else {	
									// after operator
									right.pop_back ();
									selectRightType.pop_back ();
									selectedTableRight.pop_back ();
									selectedTableRight.push_back (last);
									selectingInParticularTable = true;
								}
								instruction.popTermTokens ();
								step = where;
							} else if (current == "'" || catchcomma) {
								if (catchcomma) {
									if (current == "'") { // end of ''
										if (assigning) {
											right.push_back (*attach);
											//selectRightType.pop_back ();
											selectRightType.push_back (1);
											selectedTableRight.push_back ("");
											instruction.popTermTokens ();
											//assigning = false;
											catchcomma = false;
											step = where;
										} else {
											left.push_back (*attach);
											//selectRightType.pop_back ();
											selectLeftType.push_back (1);
											selectedTableLeft.push_back ("");
											instruction.popTermTokens ();
											//assigning = false;
											catchcomma = false;
											step = where;
										}
									} else {
										if (*attach != "")
											*attach += " ";
										*attach += current;
										instruction.popTermTokens ();
										catchcomma = true;
										step = where;
									}
								} else {
									attach = new string ();
									instruction.popTermTokens ();
									catchcomma = true;
								}
							} else {
								catchingNot = false;
								//parsing
								if (!assigning) {	// on the left of the operation
									left.push_back (current);
									// if is digit -> int
									// else -> attribute
									// if is char -> 會被抓
									if (selectingInParticularTable) {
										selectLeftType.push_back (2);
										selectingInParticularTable = false;
									} else {
										if (isdigit(current[0])) {
												selectLeftType.push_back (0);
										} else
											selectLeftType.push_back (2);
										
										selectedTableLeft.push_back ("");
									}
								} else {	// after catching the operator, come here
											// notice that this case happens when we don't have ''
									right.push_back (current);
									if (selectingInParticularTable) {
										selectRightType.push_back (2);
										selectingInParticularTable = false;
									} else{
										if (isdigit(current[0])) {
												selectRightType.push_back (0);
										} else
											selectRightType.push_back (2);
										selectedTableRight.push_back ("");
									}
								}
								instruction.popTermTokens ();
								step = where;
							}

							last = current;
							break;
						}
						default : {
							cout << "Into default" << endl;
							break;
						}
					}
					//last = current;
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
			// checking if alias exists
			int sizeForStart = startTableNames.size ();
			int sizeForFrom = fromTableNames.size ();
			int sizeForWhere = left.size ();

			if (sizeForWhere != (int)operation.size()) {
				cout << "last argument was a case without expression!!!!!!!!!!!!!!!!" << endl;
				selectedTableRight.push_back ("");
				right.push_back ("");
				selectRightType.push_back (-1);
				operation.push_back ("NULL");
			}

			bool aliasExists = true;

			for (int i=0; i<(int)fromTableNames.size(); i++) {
				select->tableNames.push_back (fromTableNames[i]);
			}

			for (int i=0; i<sizeForStart; i++) {
				select->selectedAttributesNames.push_back (startAttributeNames[i]);	// fill in attributes
				if (startTableNames[i] != "") {
					aliasExists = false;
					for (int j=0; j<sizeForFrom; j++) {
						if (startTableNames[i] == fromTableShorthands[j] || startTableNames[i] == fromTableNames[j]) {
							select->selectedAttributesTables.push_back (fromTableNames[j]);
							startTableNames[i] = fromTableNames[j];
							aliasExists = true;
							break;
						}
					}
					if (!aliasExists) {
						cout << "in start, no such alias as : " << startTableNames[i] << endl;
					}
				} else {
					select->selectedAttributesTables.push_back ("");
				}
			}
			aliasExists = true;

			for (int i=0; i<sizeForWhere; i++) {
				select->WHERE_FirstAttrNames.push_back (left[i]);	// fill in attributes
				select->WHERE_FirstTypes.push_back (selectLeftType[i]);
				if (selectedTableLeft[i] != "") {
					aliasExists = false;
					for (int j=0; j<sizeForFrom; j++) {
						if (selectedTableLeft[i] == fromTableShorthands[j] || selectedTableLeft[i] == fromTableNames[j]) {
							select->WHERE_FirstAttrTables.push_back (fromTableNames[j]);
							selectedTableLeft[i] = fromTableNames[j];
							aliasExists = true;
							break;
						}
					}
					if (!aliasExists) {
						cout << "in left, no such alias as : " << selectedTableLeft[i] << endl;
					}
				} else {
					select->WHERE_FirstAttrTables.push_back ("");
					/*
					if (select->tableNames.size() == 1) {
						select->WHERE_FirstAttrTables.push_back (select->tableNames[0]);
					} else {
						select->WHERE_FirstAttrTables.push_back (select->tableNames[0]);
					}
					*/
				}
			}
			aliasExists = true;

			for (int i=0; i<sizeForWhere; i++) {
				select->WHERE_SecondAttrNames.push_back (right[i]);	// fill in attributes
				select->WHERE_SecondTypes.push_back (selectRightType[i]);
				//select->WHERE_ExprTypes.push_back (operation[i]);
				if (operation[i] == "=") 
					select->WHERE_ExprTypes.push_back (0);
				else if (operation[i] == "<>") 
					select->WHERE_ExprTypes.push_back (1);
				else if (operation[i] == "<") 
					select->WHERE_ExprTypes.push_back (2);
				else if (operation[i] == ">") 
					select->WHERE_ExprTypes.push_back (3);
				else if (operation[i] == "NULL") 
					select->WHERE_ExprTypes.push_back (-1);
				
				else 
					cout << "I don't know what went wrong in operations!!!!!!!!!!!!!!!!!!" << endl;
				
				
				if (selectedTableRight[i] != "") {
					aliasExists = false;
					for (int j=0; j<sizeForFrom; j++) {
						if (selectedTableRight[i] == fromTableShorthands[j] || selectedTableRight[i] == fromTableNames[j]) {
							select->WHERE_SecondAttrTables.push_back (fromTableNames[j]);
							selectedTableRight[i] = fromTableNames[j];
							aliasExists = true;
							break;
						}
					}
					if (!aliasExists) {
						cout << "in right, no such alias as : " << selectedTableRight[i] << endl;
					}
				} else {
					select->WHERE_SecondAttrTables.push_back ("");
					/*
					if (select->tableNames.size() == 1) {
						select->WHERE_SecondAttrTables.push_back (select->tableNames[0]);
					} else {
						select->WHERE_SecondAttrTables.push_back (select->tableNames[0]);
					}
					*/
				}
			}
			aliasExists = true;

			for (int i=0; i<(int)select->selectedAttributesNames.size(); i++) {
				bool starFlag = false;
				if (select->selectedAttributesNames[i] == "*") {
					for (int j=0; j<(int)select->tableNames.size(); j++) {
						//cout << "in *************" << endl;
						//cout << select->selectedAttributesTables[i] << ' ' <<  select->tableNames[j] << endl;
						if (select->selectedAttributesTables[i] == select->tableNames[j]) {
							starFlag = true;
							select->isSelectAllAttrs[j] = true;
							break;
						}
						if (select->selectedAttributesTables[i] == "" && select->tableNames.size() == 1) {
							// select all and without assigning certain table
							// and has only one 'from' table
							starFlag = true;
							select->isSelectAllAttrs[j] = true;
							break;
						} else {
							cout << "select star without saying which table, and has multiple tables" << endl;
							select->isValid = false;
							return select;
						}
					}
					if (!starFlag) {
						cout << "************ i don't know what went wrong " << select->selectedAttributesTables[i]<< endl;
					}
				}
			}
			//cout << "end checking alias names" << endl;
/*
			// generate messages
			cout << "+++++++++++++++++++++" << endl;
			cout << "start table messages : ";
			cout << startTableNames.size() << ' ' << select->selectedAttributesTables.size () 
						<< ' ' << select->selectedAttributesNames.size () << endl;
			for (int i=0; i<(int)select->selectedAttributesTables.size(); i++)
			{
				cout << select->selectedAttributesTables[i] << " : " << select->selectedAttributesNames[i] << endl;
			}
			cout << endl;
			
			cout << "number of tables involed : " << select->tableNames.size() << ' ' << "select all : ";
			for (int i=0; i<(int)fromTableNames.size(); i++) {
				cout << select->tableNames[i] << ' ' << select->isSelectAllAttrs[i] << endl;
			}
			cout << endl;

			cout << "where table messages" << endl;
			//cout << select->WHERE_FirstAttrTables.size () << " vs " << select->WHERE_SecondAttrNames.size();
			for (int i=0; i<(int)select->WHERE_FirstTypes.size(); i++) {
				cout << select->WHERE_FirstAttrTables[i] << " : " << select->WHERE_FirstAttrNames[i] << " " 
						<< select->WHERE_FirstTypes[i] << " " << operation[i] << ' ' << select->WHERE_ExprTypes[i] << ' '
						<< select->WHERE_SecondAttrTables[i] << " : " << select->WHERE_SecondAttrNames[i] << " " 
						<< select->WHERE_SecondTypes[i] << endl;
			}
			cout << "---------------------" << endl;
*/
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