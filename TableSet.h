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
		
	private:
		
		vector<Table> tableVector;
};
#endif