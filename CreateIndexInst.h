#ifndef CREATEINSTINST_H
#define CREATEINSTINST_H

#include <iostream>
#include <string>
#include <vector>
#include "Instruction.h"

using namespace std;

class CreateIndexInst : public Instruction {
	public:
		
		CreateIndexInst();
		
		~CreateIndexInst(){};
		
		string tableName;
		
		vector<string> attrName;
		
		int IdxType;	//1 = B+ tree, 2 = Hash
};

#endif