#include <iostream>
#include "CreateInst.h"

CreateInst::CreateInst()
{
	tableName = "\0";	//Check whether table name is duplicate in TableSet.
	type = CREATE_TABLE;
}

CreateInst::CreateInst(string const name)
{
	tableName = name;	//Check whether table name is duplicate in TableSet.
	type = CREATE_TABLE;
}