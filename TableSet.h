#ifndef TABLESET_H
#define TABLESET_H

#include <iostream>
#include <string>
#include <vector>
#include "Table.h"

using namespace std;

class TableSet {
	public:
		
		TableSet(){tableVector.clear();};
		
		~TableSet(){tableVector.clear();};
		
		//---------------
		// void PushTable(Table)
		//		Push a new table into array.
		//---------------
		void PushTable(Table);
		
		//---------------
		// int SearchTable(string)
		//		Search table in tableVector by name and return index.
		//---------------
		int SearchTable(string);
		
		//---------------
		// bool ContainTable(string)
		//		Check table contain in tableVector or not.
		//---------------
		bool ContainTable(string);
		
		//---------------
		// bool ContainTables(vector<string>)
		//		Check tables contain in tableVector or not.
		//---------------
		bool ContainTables(vector<string>);
		
		//---------------
		// Table* GetTable(string)
		//		Get table pointer by name.
		//---------------
		Table* GetTable(string);
		
		//---------------
		// Table* GetTable(int)
		//		Get table pointer by index.
		//---------------
		Table* GetTable(int);
		
		//---------------
		// bool CheckDuplicateTable(CreateInst*)
		//		Check whether CREATE is valid by duplicate table name.
		//---------------
		bool CheckDuplicateTable(CreateInst*);
		
		//---------------
		// void ShowTables()
		//		Show all tables content in TableSet.
		//---------------
		void ShowTables();
		
		//---------------
		// Table SelectTable()
		//		Select a new Table by SelectInst.
		//---------------
		Table SelectTable(SelectInst*);
		
	private:
		
		vector<Table> tableVector;
};

#endif