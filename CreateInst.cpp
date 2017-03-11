#include <iostream>
#include "CreateInst.h"

CreateInst::CreateInst()
{
	tableName = "\0";
		
	for (int i=0; i<10; i++)
		attributeNames[i] = "\0";	
	for (int i=0; i<10; i++)
		attributeTypes[i] = -1;	//Decide whether type is valid in Parser.
							// 0 = int, 1 = varchar
	for (int i=0; i<10; i++)
		varCharSizes[i] = -1;	//Decide whether size is valid in Parser.
	for (int i=0; i<10; i++)
		isPK[i] = false;	//Decide whether PK or not in Parser.		
	attributeNum = -1;
}

CreateInst::CreateInst(string const name)
{
	tableName = name;
		
	for (int i=0; i<10; i++)
		attributeNames[i] = "\0";	
	for (int i=0; i<10; i++)
		attributeTypes[i] = -1;	//Decide whether type is valid in Parser.
							// 0 = int, 1 = varchar
	for (int i=0; i<10; i++)
		varCharSizes[i] = -1;	//Decide whether size is valid in Parser.
	for (int i=0; i<10; i++)
		isPK[i] = false;	//Decide whether PK or not in Parser.		
	attributeNum = -1;
}