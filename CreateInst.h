#ifndef CREATEINST_H
#define CREATEINST_H

#include <iostream>
#include <string>
#include "Instruction.h"

using namespace std;

class CreateInst : public Instruction {
	public:
		
		CreateInst();
		
		string tableName;
		
		string attributeNames[10];
		
		int attributeTypes[10];	//Decide whether type is valid in Parser.
								// 0 = int, 1 = varchar
		int varCharSizes[10];	//Decide whether size is valid in Parser.
		
		bool isPK[10];	//Decide whether PK or not in Parser.
		
		int attributeNum;
		
	private:
		
		
};
#endif