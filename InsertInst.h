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
		
		string attributesNames[10];
		
		string attributesValues[10];
		
	private:
		
		
};
#endif