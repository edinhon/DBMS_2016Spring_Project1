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
// bool SelectTable()
//		Select a new Table by SelectInst and push into tableVector.
//		Before call this function, 
//		it should call main.CheckSelectAttrTableName() and ContainTables().
//-----------------------------------------
bool TableSet::SelectTable(SelectInst* sinst)
{
	vector<Table*> selectedTables;
	for(int i = 0 ; i < (int)sinst->tableNames.size() ; i++){
		Table* t = GetTable(sinst->tableNames[i].getTableName());
		selectedTables.push_back(t);
	}
	
	//檢查沒有table name的attribute是否都出現在FROM的tables
	for(int i = 0 ; i < (int)sinst->selectedAttributesNames.size() ; i++){
		if(!sinst->isSelectedAttributesTables[j]){
			bool flag = false;
			for(int j = 0 ; j < (int)selectedTables.size() ; j++){
				if(selectedTables[j].ContainAttribute(sinst->selectedAttributesNames[i]))
					flag = true;
				else flag = false;
			}
			if(flag){
				cout << "- Error: The attribute " << sinst->selectedAttributesNames[i] <<
					" is an ambiguous attribute between two tables, but not used as a prefix in the attribute.\n";
				return false;
			}
		}
	}
	
	if (!sinst->isWHERE){
		
		Table returnT();
		
		if(sinst->isSelectAllAttrs){
			
		} 
		else {
			//依序檢查每個table
			for(int i = 0 ; i < (int)selectedTables.size() ; i++){
				
				//取得此table的alias name
				string tableAlias = "";
				if(sinst->isTableNameAlias[i]){
					for(int j = 0 ; j < (int) sinst->tableNameAliasIndex.size() ; j++){
						if(sinst->tableNameAliasIndex[j] == i){
							tableAlias = sinst->tableNameAlias[j];
						}
					}
				}
				
				//檢查Attribute是否屬於這個table並取出Attribute
				for(int j = 0 ; j < (int)sinst->selectedAttributesNames.size() ; j++){
					//分成有table name或沒table name
					if(sinst->isSelectedAttributesTables[j]){
						
						//取得table (alias) name
						string TName;
						for(int k = 0 ; k < (int)sinst->selectedAttributesTablesIndex.size() ; k++){
							if(sinst->selectedAttributesTablesIndex[k] == j){
								TName = sinst->selectedAttributesTables[k];
								break;
							}
						}
						
						//檢查table name & alias是否屬於這個table
						bool isThisTable = false;
						string n1 = TName;
						transform(n1.begin(), n1.end(), n1.begin(),::tolower);
						string n2 = selectedTables[i]->getTableName();
						transform(n2.begin(), n2.end(), n2.begin(),::tolower);
						
						if(n1.compare(n2) == 0) isThisTable = true;
						
						if(sinst->isTableNameAlias[i]){
							n2 = tableAlias;
							transform(n2.begin(), n2.end(), n2.begin(),::tolower);
							if(n1.compare(n2) == 0) isThisTable = true;
						}
						
						//如果此attribute table屬於此table, 取出attribute放進新table
						if(isThisTable){
							//檢查此attribute是否存在於此table
							if(selectedTables[i]->ContainAttribute(sinst->selectedAttributesNames[j])){
								if(!returnT.InsertAttribute(selectedTables[i], sinst->selectedAttributesNames[j]))
									return false;
							} else {
								cout << "- Error: The attribute " << sinst->selectedAttributesNames[j] << 
									" doesn't exist in the table " << selectedTables[i]->getTableName() << " .\n";
								return false;
							}
						}
						
					} 
					else {
						//檢查此attribute是否存在於此table, 是則取出attribute放進新table
						if(selectedTables[i]->ContainAttribute(sinst->selectedAttributesNames[j])){
							if(!returnT.InsertAttribute(selectedTables[i]->GetAttribute(sinst->selectedAttributesNames[j])))
								return false;
						} else {
							cout << "- Error: The attribute " << sinst->selectedAttributesNames[j] << 
								" doesn't exist in the table " << selectedTables[i]->getTableName() << " .\n";
							return false;
						}
					}
				}
				
			}
		}
		
	} 
	else {
		
	}
}




