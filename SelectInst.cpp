#include <iostream>
#include "SelectInst.h"

SelectInst::SelectInst()
{
	isValid = false;

	//tableName = "\0";
	
	type = SELECTION;
	
	isSelectAllAttrs[0] = false;
	isSelectAllAttrs[1] = false;
	
	isWHERE = false;
	
	isCOUNT = false;
		
	isSUM = false;
}

SelectInst::SelectInst(string const name)
{
	isValid = false;
	
	//tableName = name;
	
	type = SELECTION;
	
	isSelectAllAttrs[0] = false;
	isSelectAllAttrs[1] = false;
		
	isWHERE = false;
	
	isCOUNT = false;
		
	isSUM = false;
}
/*
vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}*/