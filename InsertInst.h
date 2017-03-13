#ifndef INSERTINST_H
#define INSERTINST_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Instruction.h"

using namespace std;

class InsertInst : public Instruction {
	public:
		
		InsertInst();
		InsertInst(string const name);
		
		~InsertInst(){};
		
		string tableName;	//Check whether table name is duplicate in TableSet.
		
		bool isWithName;	//Check whether INSERT is with attribute name or not in Parser.
		
		vector<string> insertedAttributes;	//Decide whether attribute names are duplicate in Parser.
		
		vector<string*> insertedValues;	//value of inserted attributes
		
		vector<int> insertedValueTypes;	//Check the value type by '' or not in Parser.
										//0 = int, 1 = varchar
	private:
		
		
};
#endif