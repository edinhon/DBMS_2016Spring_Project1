#include <iostream>
#include "TableSet.h"

using namespace std;

//---------------
// void PushTable(Table)
//		Push a new table into array.
//---------------
void TableSet::PushTable(Table t)
{
	tableVector.push_back(t);
}

//---------------
// int SearchTable(string)
//		Search table in tableVector by name and return index.
//---------------
int TableSet::SearchTable(string tableName)
{
	for (vector<Table>::iterator it = tableVector.begin(); it != tableVector.end(); it++){
		if(it->getTableName().compare(tableName) == 0){
			return (it - tableVector.begin());
		}
	}
	
	cout << "Error: Cannot find table" << tableName << endl;
	return -1;
}

//---------------
// Table* GetTable(string)
//		Get table pointer by name.
//---------------
Table* TableSet::GetTable(string tableName)
{
	for (int i = 0 ; i < (int)tableVector.size() ; i++){
		if(tableVector[i].getTableName().compare(tableName) == 0){
			return &tableVector[i];
		}
	}
	
	cout << "Error: Cannot find table" << tableName << endl;
	return nullptr;
}

//---------------
// Table* GetTable(int)
//		Get table pointer by index.
//---------------
Table* TableSet::GetTable(int index)
{
	if((int)tableVector.size() > index)
		return &tableVector[index];
	
	cout << "Error: Cannot find table of index" << index << endl;
	return nullptr;
}

//---------------
// bool CheckDuplicateTable(CreateInst*)
//		Check whether CREATE is valid by duplicate table name.
//---------------
bool CheckDuplicateTable(CreateInst* cinst)
{
	for (vector<Table>::iterator it = tableVector.begin(); it != tableVector.end(); it++){
		if(it->getTableName().compare(cinst.tableName) == 0){
			return false;
		}
	}
	return true;
}


