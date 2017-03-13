#include <iostream>
#include "InsertInst.h"

InsertInst::InsertInst()
{
	isValid = true;
	tableName = "\0";
		
	for (int i=0; i<10; i++)
		attributeNames[i] = "\0";	
	for (int i=0; i<10; i++)
		attributeValues[i] = "\0";
	for (int i=0; i<10; i++)
		attributeValueTypes[i] = -1;	//Decide whether type is valid in Parser.
							// 0 = int, 1 = varchar
	
	isWithName = false;	//Check whether INSERT is with attribute name or not in Parser.
		
	attributeNameNum = -1;	//If without attribute name, set this to -1.
		
	attributeValueNum = -1;	//Decide in Parser. 
							//Note: this variable should be equal to attributeNameNum.
	
	type = INSERT_TUPLE;
}

InsertInst::InsertInst(string const name)
{
	isValid = true;
	tableName = name;
		
	for (int i=0; i<10; i++)
		attributeNames[i] = "\0";	
	for (int i=0; i<10; i++)
		attributeValues[i] = "\0";
	for (int i=0; i<10; i++)
		attributeValueTypes[i] = -1;	//Decide whether type is valid in Parser.
							// 0 = int, 1 = varchar
	
	isWithName = false;	//Check whether INSERT is with attribute name or not in Parser.
		
	attributeNameNum = -1;	//If without attribute name, set this to -1.
		
	attributeValueNum = -1;	//Decide in Parser. 
							//Note: this variable should be equal to attributeNameNum.
	type = INSERT_TUPLE;
}