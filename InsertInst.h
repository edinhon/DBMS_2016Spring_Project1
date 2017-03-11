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
		
		string tableName;
		
		string attributeNames[10];
		
		string attributeValues[10];
		
		bool isWithName;
		
		int attributeNameNum;
		
		int attributeValueNum;
		
	private:
		
		
};
#endif