#include <iostream>
#include "SelectInst.h"

SelectInst::SelectInst()
{
	isValid = false;

	tableName = "\0";
	
	type = SELECTION;
}

SelectInst::SelectInst(string const name)
{
	isValid = false;
	
	tableName = name;
	
	type = SELECTION;
}