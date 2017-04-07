#ifndef SELECTINST_H
#define SELECTINST_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Instruction.h"

using namespace std;

class SelectInst : public Instruction {
	public:
		
		SelectInst();
		SelectInst(string const name);
		~SelectInst(){};
		
		vector<string> tableNames;	//SELECT table FROM these tableName
									//Check whether table name is duplicate in TableSet.	
		vector<bool> isTableNameAlias;							
									
		vector<string> tableNameAlias;	//Table Name alias
		
		vector<int> tableNameAliasIndex;	//record the index of table name alias
		
		vector<string> selectedAttributesNames;	//Decide whether attribute names are duplicate in Parser.
		
		vector<bool> isSelectedAttributesTables;//Whether a table name of attribute or not.
		
		vector<string> selectedAttributesTables;//Table name of attributes.
		
		vector<int> selectedAttributesTablesIndex;	//record the index of attribute table name alias
		
		bool isSelectAllAttrs;
		
		bool isWHERE;
		
		vector<string> WHERE_Attributes;	//WHERE clause attribute names
		
		vector<string> WHERE_Values;	//WHERE clause value to boolean expression
		
		vector<int> WHERE_ValueTypes;	//WHERE clause value type, 0 = int, 1 = varchar, 2 = attribute, -1 = error
		
		vector<int> WHERE_ExprTypes;	//WHERE clause expression type, 0 = '=', 1 = '<>', 2 = '<', 3 = '>', -1 = error
		
		int WHERE_ExprAO;	//WHERE clause two expression separated by AND or OR, 0 = AND, 1 = OR, -1 = error
		
		bool isCOUNT;
		
		vector<string> COUNT_Attributes;
		
		bool isCOUNTAllAttrs;	//total number of WHERE clause
		
		bool isSUM;
		
		vector<string> SUM_Attributes;
		
};

//vector<string> split(const string &s, char delim);
#endif