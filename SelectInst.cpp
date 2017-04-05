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

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}