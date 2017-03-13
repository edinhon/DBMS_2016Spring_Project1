#ifndef CREATEINST_H
#define CREATEINST_H

#include <iostream>
#include <string>
#include "Instruction.h"

using namespace std;

class CreateInst : public Instruction {
	public:
		
		CreateInst();
		CreateInst(string const name);
		
		~CreateInst(){};
		
		string tableName;	//Check whether table name is duplicate in TableSet.
		
		vector<string> attributeNames;	//Decide whether attribute names are duplicate in Parser.
		
		vector<int> attributeTypes;	//Decide whether type is valid in Parser.
								// 0 = int, 1 = varchar
		vector<int> varCharSizes;	//Decide whether size is valid in Parser.
		
		vector<bool> isPK;	//Decide whether PK or not in Parser.
		
	private:
		
		
};
#endif