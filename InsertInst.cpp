#include <iostream>
#include "InsertInst.h"

InsertInst::InsertInst()
{
	isValid = false;

	tableName = "\0";
	
	isWithName = false;	//Check whether INSERT is with attribute name or not in Parser.

	type = INSERT_TUPLE;
}

InsertInst::InsertInst(string const name)
{
	isValid = false;
	
	tableName = name;
	
	isWithName = false;	//Check whether INSERT is with attribute name or not in Parser.

	type = INSERT_TUPLE;
}