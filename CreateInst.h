#ifndef CREATEINST_H
#define CREATEINST_H

#include <iostream>
#include <string>
#include <vector>
#include "Instruction.h"

using namespace std;

class CreateInst : public Instruction {
	public:
		
		CreateInst();
		
		string tableName;
		
		vector<string> attributeNames;
		
		vector<string> attributeTypes;
		
		vector<bool> isPK;
		
	private:
		
		
};
#endif