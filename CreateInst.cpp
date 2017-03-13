#include <iostream>
#include "CreateInst.h"

CreateInst::CreateInst()
{
	tableName = "\0";	//Check whether table name is duplicate in TableSet.
	
	attributeNum = -1;

	type = CREATE_TABLE;
}

CreateInst::CreateInst(string const name)
{
	tableName = name;	//Check whether table name is duplicate in TableSet.
	
	attributeNum = -1;

	type = CREATE_TABLE;
}