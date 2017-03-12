#ifndef INSERTINST_H
#define INSERTINST_H

#include <iostream>
#include <string>
#include <vector>
#include "Instruction.h"

using namespace std;

class InsertInst : public Instruction {
	public:
		
		InsertInst();
		
		~InsertInst(){};
		
		string tableName;	//Check whether table name is duplicate in TableSet.
		
		string attributeNames[10];	//Decide whether attribute names are duplicate in Parser.
		
		string attributeValues[10];	//TODO: 如果是varchar，不要把''也放進來
		
		int attributeValueTypes[10];	//Check the value type by '' or not in Parser.
										//0 = int, 1 = varchar
		bool isWithName;	//Check whether INSERT is with attribute name or not in Parser.
		
		int attributeNameNum;	//If without attribute name, set this to -1.
		
		int attributeValueNum;	//Decide in Parser. 
								//Note: this variable should be equal to attributeNameNum.
		
	private:
		
		
};
#endif