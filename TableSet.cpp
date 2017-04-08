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

//---------------------------------------
// bool CheckSelectInst(SelectInst*)
//		Check SELECT is valid or not.
//---------------------------------------
bool TableSet::CheckSelectInst(SelectInst* sinst, vector<Table*> selectedTables)
{
	if(sinst->isSelectAllAttrs && sinst->isSUM){
		cout << "- Error: Cannot sum up all attributes.\n";
		return false;
	}
	
	//檢查是否table name of attribute都存在FROM裡面
	for(int i = 0 ; i < (int)sinst->selectedAttributesTables.size() ; i++){
		bool flag = false;
		string n1 = sinst->selectedAttributesTables[i];
		transform(n1.begin(), n1.end(), n1.begin(),::tolower);
		//檢查table name
		for(int j = 0 ; j < (int)sinst->tableNames.size() ; j++){
			string n2 = sinst->tableNames[j];
			transform(n2.begin(), n2.end(), n2.begin(),::tolower);
			
			if(n1.compare(n2) == 0){
				flag = true;
				break;
			}
		}
		//檢查alias
		for(int j = 0 ; j < (int)sinst->tableNameAlias.size() ; j++){
			string n2 = sinst->tableNameAlias[j];
			transform(n2.begin(), n2.end(), n2.begin(),::tolower);
			
			if(n1.compare(n2) == 0){
				flag = true;
				break;
			}
		}
		
		if(!flag){
			cout << "- Error: The table name of attribute " <<
				sinst->selectedAttributesTables[i] << " cannot be found in SELECT FROM.\n";
			return false;
		}
	}
	
	//檢查沒有table name的attribute是否都出現在FROM的tables, 以及是否都沒出現
	for(int i = 0 ; i < (int)sinst->selectedAttributesNames.size() ; i++){
		if(!sinst->isSelectedAttributesTables[i]){
			bool flag = false;
			bool flag2 = false;
			for(int j = 0 ; j < (int)selectedTables.size() ; j++){
				if(selectedTables[j]->ContainAttribute(sinst->selectedAttributesNames[i])){
					flag = true;
					flag2 = true;
				} else flag = false;
			}
			
			//都出現
			if(flag && sinst->tableNames.size() == 2){
				cout << "- Error: The attribute " << sinst->selectedAttributesNames[i] <<
					" is an ambiguous attribute between two tables, but not used as a prefix in the attribute.\n";
				return false;
			}
			
			//都沒出現
			if(!flag2){
				cout << "- Error: The attribute " << sinst->selectedAttributesNames[i] <<
					" doesn't exist in all tables.\n";
				return false;
			}
		}
	}
	
	return true;
}

//-----------------------------------------
// bool SelectTable()
//		Select a new Table by SelectInst and push into tableVector.
//		Before call this function, it should call ContainTables().
//-----------------------------------------
bool TableSet::SelectTable(SelectInst* sinst)
{
	vector<Table*> selectedTables;
	for(int i = 0 ; i < (int)sinst->tableNames.size() ; i++){
		Table* t = GetTable(sinst->tableNames[i]);
		selectedTables.push_back(t);
	}
	
	//檢查合法性
	if(!CheckSelectInst(sinst, selectedTables))
		return false;
	
	
	Table *returnT = new Table();
	
	//分成是否有WHERE
	if (!sinst->isWHERE){
		if(!SELECT_InsertAttributes(returnT, sinst, selectedTables))
			return false;
		if(!SELECT_InsertTuples(returnT, sinst, selectedTables))
			return false;
	} 
	else {
		if(!SELECT_InsertAttributes(returnT, sinst, selectedTables))
			return false;
		if(!SELECT_InsertTuplesWithWhere(returnT, sinst, selectedTables))
			return false;
	}
	tableVector.push_back(*returnT);
	return true;
}

//---------------------------------------
// bool SELECT_InsertAttributes(Table*, SelectInst*)
//		Insert attributes by SELECT.
//---------------------------------------
bool TableSet::SELECT_InsertAttributes(Table* t, SelectInst* sinst, vector<Table*> selectedTables)
{
	if(sinst->isSelectAllAttrs){
		//取出所有attributes放進新table
		for(int i = 0 ; i < (int)selectedTables.size() ; i++){
			if(!(t->CopyAttributes(selectedTables[i])))
				return false;
		}
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
							if(!(t->CopyAttribute(selectedTables[i], sinst->selectedAttributesNames[j])))
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
						if(!(t->CopyAttribute(selectedTables[i], sinst->selectedAttributesNames[j])))
							return false;
					}
				}
			}
		}
		
		//如果是SUM的話，檢查其attribute type
		if(sinst->isSUM && t->GetAttributeType(sinst->selectedAttributesNames.front()) != 0){
			cout << "- Error: Cannot sum up the attribute " << 
				sinst->selectedAttributesNames.front() << " which is not integer\n";
			return false;
		}
	}
	return true;
}

//---------------------------------------
// bool SELECT_InsertTuples(Table*, SelectInst*, vector<Table*>)
//		Insert tuples by SELECT.
//---------------------------------------
bool TableSet::SELECT_InsertTuples(Table*, SelectInst*, vector<Table*>)
{
	return true;
}

//---------------------------------------
// bool SELECT_InsertTuplesWithWhere(Table*, SelectInst*, vector<Table*>)
//		Insert tuples by SELECT with WHERE.
//---------------------------------------
bool TableSet::SELECT_InsertTuplesWithWhere(Table*, SelectInst*, vector<Table*>)
{
	return true;
}

//------------------------------
// Table* GetSelectedTable()
//		Get the table of SELECT.
//------------------------------
Table* TableSet::GetSelectedTable()
{
	return &(tableVector.back());
}

//------------------------------
// bool DeleteSelectedTable()
//		Delete the table of SELECT.
//------------------------------
bool TableSet::DeleteSelectedTable()
{
	if(tableVector.back().getTableName().compare("SelectedTable") == 0){
		tableVector.pop_back();
		return true;
	}
	cout << "- Error: The last table is not SELECT Table.\n";
	return false;
}


