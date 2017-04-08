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
		
		//---------------------------------------
		// bool CheckSelectInst(SelectInst*)
		//		Check SELECT is valid or not.
		//---------------------------------------
		bool CheckSelectInst(SelectInst*, vector<Table*>);
		
		//---------------------------------------
		// bool SelectTable()
		//		Select a new Table by SelectInst and push into tableVector.
		//---------------------------------------
		bool SelectTable(SelectInst*);
		
		//---------------------------------------
		// bool SELECT_InsertAttributes(Table*, SelectInst*, vector<Table*>)
		//		Insert attributes by SELECT.
		//---------------------------------------
		bool SELECT_InsertAttributes(Table*, SelectInst*, vector<Table*>);
		
		//---------------------------------------
		// bool SELECT_InsertTuples(Table*, SelectInst*, vector<Table*>)
		//		Insert tuples by SELECT.
		//---------------------------------------
		bool SELECT_InsertTuples(Table*, SelectInst*, vector<Table*>);
		
		//---------------------------------------
		// bool SELECT_InsertTuplesWithWhere(Table*, SelectInst*, vector<Table*>)
		//		Insert tuples by SELECT with WHERE.
		//---------------------------------------
		bool SELECT_InsertTuplesWithWhere(Table*, SelectInst*, vector<Table*>);
		
		
		
		//------------------------------
		// Table* GetSelectedTable()
		//		Get the table of SELECT.
		//------------------------------
		Table* GetSelectedTable();
		
		//------------------------------
		// bool DeleteSelectedTable()
		//		Delete the table of SELECT.
		//------------------------------
		bool DeleteSelectedTable();
		
	private:
		
		vector<Table> tableVector;
};

#endif