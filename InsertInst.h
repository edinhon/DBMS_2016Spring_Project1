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
		
		string tableName;
		
		vector<string> attributeNames[10];
		
		vector<string> attributeValues[10];
		
		bool isWithName;
		
	private:
		
		
};
#endif