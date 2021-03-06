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
		
		// from
		// check
		vector<string> tableNames;	//FROM table name，檢查是否重複	
		
		// select
		// check
		vector<string> selectedAttributesNames;	//SELECT 的 attribute name
		// check
		vector<string> selectedAttributesTables;//attribute 的 table name, "" = not exist
		// check
		bool isSelectAllAttrs[2];	//是否(*)
		// check
		bool isWHERE;	//是否有WHERE
		
		// where
		// check
		vector<string> WHERE_FirstAttrNames;	//WHERE的first attributes or values, ex: WHERE t."attr" = t.attr, "" = not exist
		// check
		vector<string> WHERE_FirstAttrTables;	//WHERE的first table names, ex: WHERE "t".attr = t.attr, "" = not exist
		// check
		vector<int> WHERE_FirstTypes;	//WHERE clause value type, 0 = int, 1 = varchar, 2 = attribute, -1 = not exist
		// check
		vector<string> WHERE_SecondAttrNames;	//WHERE的first attributes or values, ex: WHERE t.attr = t."attr", "" = not exist
		// check
		vector<string> WHERE_SecondAttrTables;	//WHERE的first table names, ex: WHERE t.attr = "t".attr, "" = not exist
		// check
		vector<int> WHERE_SecondTypes;	//WHERE clause value type, 0 = int, 1 = varchar, 2 = attribute, -1 = not exist
		// check
		vector<int> WHERE_ExprTypes;	//WHERE clause expression type, 0 = '=', 1 = '<>', 2 = '<', 3 = '>', -1 = not exist
		// check
		int WHERE_ExprAO;	//WHERE clause two expression separated by AND or OR, 0 = AND, 1 = OR, -1 = not exist
		// check
		bool isCOUNT;
		// check
		bool isSUM;
		
};

//vector<string> split(const string &s, char delim);
#endif