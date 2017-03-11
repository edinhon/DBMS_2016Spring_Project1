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
		
		string attributeNames[10];	//Decide whether attribute names are duplicate in Parser.
		
		int attributeTypes[10];	//Decide whether type is valid in Parser.
								// 0 = int, 1 = varchar
		int varCharSizes[10];	//Decide whether size is valid in Parser.
		
		bool isPK[10];	//Decide whether PK or not in Parser.
		
		int attributeNum;	//Check whether attribute number is <= 10 in Parser.
		
	private:
		
		
};
#endif