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
		
		string attributesNames[10];
		
		string attributesTypes[10];
		
		bool isPK[10];
		
	private:
		
		
};
#endif