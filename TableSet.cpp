#include <iostream>
#include <algorithm>
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
// bool ContainTable(string)
//		Check table contain in tableVector or not.
//---------------
bool TableSet::ContainTable(string tableName)
{
	string n1 = tableName;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for (vector<Table>::iterator it = tableVector.begin(); it != tableVector.end(); it++){
		string n2 = it->getTableName();
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0){
			return true;
		}
	}
	
	cout << "- Error: Cannot find table " << tableName << endl;
	return false;
}

//---------------
// bool ContainTables(vector<string>)
//		Check tables contain in tableVector or not.
//---------------
bool TableSet::ContainTables(vector<string> tableNames)
{
	for (vector<Table>::iterator it = tableVector.begin(); it != tableVector.end(); it++){
		if(!ContainTable(it->getTableName())){
			return false;
		}
	}
	
	return true;
}

//---------------
// int SearchTable(string)
//		Search table in tableVector by name and return index.
//---------------
int TableSet::SearchTable(string tableName)
{
	string n1 = tableName;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for (vector<Table>::iterator it = tableVector.begin(); it != tableVector.end(); it++){
		string n2 = it->getTableName();
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0){
			return (it - tableVector.begin());
		}
	}
	
	cout << "- Error: Cannot find table " << tableName << endl;
	return -1;
}

//---------------
// Table* GetTable(string)
//		Get table pointer by name.
//---------------
Table* TableSet::GetTable(string tableName)
{
	string n1 = tableName;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for (int i = 0 ; i < (int)tableVector.size() ; i++){
		string n2 = tableVector[i].getTableName();
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0){
			return &tableVector[i];
		}
	}
	
	cout << "- Error: Cannot find table" << tableName << endl;
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
	
	cout << "- Error: Cannot find table of index" << index << endl;
	return nullptr;
}

//---------------
// bool CheckDuplicateTable(CreateInst*)
//		Check whether CREATE is valid by duplicate table name.
//---------------
bool TableSet::CheckDuplicateTable(CreateInst* cinst)
{
	string n1 = cinst->tableName;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for (vector<Table>::iterator it = tableVector.begin(); it != tableVector.end(); it++){
		string n2 = it->getTableName();
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0){
			cout << "- Error: There exists a table with the same name of this CREATE instruction\n";
			return false;
		}
	}
	return true;
}

//---------------
// void ShowTables()
//		Show all tables content in TableSet.
//---------------
void TableSet::ShowTables()
{
	for (int i = 0 ; i < (int)tableVector.size() ; i++){
		cout << endl;
		tableVector[i].ShowTable();
	}
}

//-----------------------------------------
// Table SelectTable()
//		Select a new Table by SelectInst.
//-----------------------------------------
Table TableSet::SelectTable(SelectInst* sinst)
{
	vector<Table*> selectedTables;
	for(int i = 0 ; i < (int)sinst->tableNames.size() ; i++){
		Table* t = GetTable(sinst->tableNames[i].getTableName());
		selectedTables.push_back(t);
	}
	
	if (!sinst->isWHERE){
		
		if(sinst->isSelectAllAttrs){
			
		} else {
			for(int i = 0 ; i < (int)selectedTables.size() ; i++){
			
				
				
			}
		}
		
	} else {
		
	}
}




