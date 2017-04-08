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
		
		vector<string> tableNames;	//FROM table name
									//檢查是否重複	
		vector<bool> isTableNameAlias;	//是否有alias name							
									
		vector<string> tableNameAlias;	//Table Name alias
		
		vector<int> tableNameAliasIndex;	//紀錄alias name對應哪個table name
		
		vector<string> selectedAttributesNames;	//SELECT 的 attribute name
		
		vector<bool> isSelectedAttributesTables;//attribute是否有table name, ex: t.attribute
		
		vector<string> selectedAttributesTables;//attribute 的 table name
		
		vector<int> selectedAttributesTablesIndex;	//紀錄attribute table name對應哪個attribute
		
		bool isSelectAllAttrs;	//是否(*)
		
		bool isWHERE;	//是否有WHERE
		
		vector<string> WHERE_FirstAttrNames;	//WHERE的first attributes or values, ex: WHERE t."attr" = t.attr
		
		vector<string> WHERE_FirstIsTables;	//WHERE的first attributes or values是否有table name
		
		vector<string> WHERE_FirstAttrTables;	//WHERE的first table names, ex: WHERE "t".attr = t.attr
		
		vector<int> WHERE_FirstTypes;	//WHERE clause value type, 0 = int, 1 = varchar, 2 = attribute, -1 = error
		
		vector<string> WHERE_SecondAttrNames;	//WHERE的first attributes or values, ex: WHERE t.attr = t."attr"
		
		vector<string> WHERE_SecondtIsTables;	//WHERE的first attributes or values是否有table name
		
		vector<string> WHERE_SecondAttrTables;	//WHERE的first table names, ex: WHERE t.attr = "t".attr
		
		vector<int> WHERE_SecondTypes;	//WHERE clause value type, 0 = int, 1 = varchar, 2 = attribute, -1 = error
		
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