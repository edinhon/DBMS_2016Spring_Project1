#include <iostream>
#include <algorithm>
#include <cstdlib>
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
	for (int i = 0 ; i < (int)tableNames.size() ; i++){
		if(!ContainTable(tableNames[i])){
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
	//檢查SUM不能以*選全部attributes
	/*if((sinst->isSelectAllAttrs[0] || sinst->isSelectAllAttrs[1]) && sinst->isSUM){
		cout << "- Error: Cannot sum up all attributes.\n";
		return false;
	}*/
	
	//檢查是否table name of attribute都存在FROM裡面
	for(int i = 0 ; i < (int)sinst->selectedAttributesTables.size() ; i++){
		// 如果沒有特別 assign 就跳過
		if (sinst->selectedAttributesTables[i].compare("") == 0)
			continue;
		
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
		
		if(!flag){
			cout << "- Error: The table name of attribute " <<
				sinst->selectedAttributesTables[i] << " cannot be found in SELECT FROM.\n";
			return false;
		}
	}
	
	//檢查沒有table name的attribute是否都出現在FROM的tables, 以及是否都沒出現
	for(int i = 0 ; i < (int)sinst->selectedAttributesNames.size() ; i++){
		// select all 的話就不用檢查了
		if (sinst->isSelectAllAttrs[i]) 
			continue;

		if(sinst->selectedAttributesTables[i].compare("") == 0){
			bool flags [2] = {false, false};
			for(int j = 0 ; j < (int)selectedTables.size() ; j++){
				if(selectedTables[j]->ContainAttribute(sinst->selectedAttributesNames[i])){
					flags[j] = true;
				} 
			}
			
			//都出現
			if (flags[0] && flags[1] && sinst->tableNames.size() == 2) {
				cout << "- Error: The attribute " << sinst->selectedAttributesNames[i] <<
					" is an ambiguous attribute between two tables, but not used as a prefix in the attribute.\n";
				return false;
			}
			
			//都沒出現
			if (!flags[0] && !flags[1]) {
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
	vector<int> *TIndex = new vector<int>((int)sinst->selectedAttributesNames.size(), -1);
	
	//分成是否有WHERE
	if (!sinst->isWHERE){
		if(!SELECT_InsertAttributes(returnT, sinst, selectedTables, TIndex))
			return false;
		/*for(int i = 0 ; i < (int)TIndex->size() ; i++){
			cout << "TIndex "<< i << " = " << TIndex->at(i) <<endl;
		}*/
		if(!SELECT_InsertTuples(returnT, sinst, selectedTables, TIndex))
			return false;
		//cout << "hi" << endl;
	} 
	else {
		if(!SELECT_InsertAttributes(returnT, sinst, selectedTables, TIndex))
			return false;
		/*for(int i = 0 ; i < (int)TIndex->size() ; i++){
			cout << "TIndex "<< i << " = " << TIndex->at(i) <<endl;
		}*/
		if(!SELECT_InsertTuplesWithWhere(returnT, sinst, selectedTables, TIndex))
			return false;
	}
	tableVector.push_back(*returnT);
	return true;
}

//---------------------------------------
// bool SELECT_InsertAttributes(Table*, SelectInst*, vector<Table*>, vector<int>*)
//		Insert attributes by SELECT.
//---------------------------------------
bool TableSet::SELECT_InsertAttributes(Table* t, SelectInst* sinst, vector<Table*> selectedTables, vector<int>* TIndex)
{
	if(sinst->isSelectAllAttrs[0] || sinst->isSelectAllAttrs[1]) {
		TIndex->clear();
		for(int i = 0 ; i < (int)selectedTables.size() ; i++){
			if(sinst->isSelectAllAttrs[i]){
				if(!(t->CopyAttributes(selectedTables[i], i)))
					return false;
				for(int j = 0 ; j < (int)selectedTables[i]->attributes.size() ; j++){
					TIndex->push_back(i);
				}
			}
		}
	}
	else {
		//依序檢查每個table
		for(int i = 0 ; i < (int)selectedTables.size() ; i++){
			
			//檢查Attribute是否屬於這個table並取出Attribute
			for(int j = 0 ; j < (int)sinst->selectedAttributesNames.size() ; j++){
				//分成有table name或沒table name
				if(sinst->selectedAttributesTables[j].compare("") != 0){
					
					//檢查table name是否屬於這個table
					bool isThisTable = false;
					string n1 = sinst->selectedAttributesTables[j];
					transform(n1.begin(), n1.end(), n1.begin(),::tolower);
					string n2 = selectedTables[i]->getTableName();
					transform(n2.begin(), n2.end(), n2.begin(),::tolower);

					if(n1.compare(n2) == 0) isThisTable = true;
					
					//如果此attribute table屬於此table, 取出attribute放進新table
					if(isThisTable){
						//檢查此attribute是否存在於此table
						if(selectedTables[i]->ContainAttribute(sinst->selectedAttributesNames[j])){
							if(!(t->CopyAttribute(selectedTables[i], sinst->selectedAttributesNames[j], i))){
								return false;
							} else {
								TIndex->at(j) = i;
							}
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
						if(!(t->CopyAttribute(selectedTables[i], sinst->selectedAttributesNames[j], i))){
							return false;
						} else {
							TIndex->at(j) = i;
						}
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
// bool SELECT_InsertTuples(Table*, SelectInst*, vector<Table*>, vector<int>*)
//		Insert tuples by SELECT.
//---------------------------------------
bool TableSet::SELECT_InsertTuples(Table* t, SelectInst* sinst, vector<Table*> selectedTables, vector<int>* TIndex)
{
	if (selectedTables.size() == 1){

		for(int i = 0 ; i < (int)selectedTables[0]->tuples.size() ; i++){
			
			int tupleIndex = t->InsertEmptyTuple();
			
			if(sinst->isSelectAllAttrs[0]){
				if(!t->CopyValuesToTuple(selectedTables[0], tupleIndex, i))
					return false;
			}
			else {
				
				for(int j = 0 ; j < (int)sinst->selectedAttributesNames.size() ; j++){
					//此attribute屬於此table
					if(TIndex->at(j) == 0){
						if(!t->CopyValueToTuple(selectedTables[0], sinst->selectedAttributesNames[j], tupleIndex, i))
							return false;
					}
				}
			}
			
			//如果沒有加入任何attribute value，要刪掉此empty tuple
			if(t->tuples[tupleIndex].CheckEmpty()){
				t->tuples.pop_back();
			}
		}
	} else if (selectedTables.size() == 2){
		
		if(selectedTables[0]->tuples.size() == 0){
			vector<Table*> tempTables;
			tempTables.push_back(selectedTables[1]);
			if(!SELECT_InsertTuples(t, sinst, tempTables, TIndex))
				return false;
		} else if(selectedTables[1]->tuples.size() == 0){
			vector<Table*> tempTables;
			tempTables.push_back(selectedTables[0]);
			if(!SELECT_InsertTuples(t, sinst, tempTables, TIndex))
				return false;
		} else{
			
			for(int i = 0 ; i < (int)selectedTables[0]->tuples.size() ; i++){
				for(int j = 0 ; j < (int)selectedTables[1]->tuples.size() ; j++){
					
					int tupleIndex = t->InsertEmptyTuple();
					
					if(sinst->isSelectAllAttrs[0] || sinst->isSelectAllAttrs[1]){
						if(sinst->isSelectAllAttrs[0]){
							if(!t->CopyValuesToTuple(selectedTables[0], tupleIndex, i))
								return false;
						}
						if(sinst->isSelectAllAttrs[1]){
							if(!t->CopyValuesToTuple(selectedTables[1], tupleIndex, j))
								return false;
						}
					}
					else {
						for(int k = 0 ; k < (int)sinst->selectedAttributesNames.size() ; k++){
							//此attribute屬於First table
							if(TIndex->at(k) == 0){
								if(!t->CopyValueToTuple(selectedTables[0], sinst->selectedAttributesNames[k], tupleIndex, i))
									return false;
							}
							//此attribute屬於Second table
							if(TIndex->at(k) == 1){
								if(!t->CopyValueToTuple(selectedTables[1], sinst->selectedAttributesNames[k], tupleIndex, j))
									return false;
							}
						}
					}
					
					//如果沒有加入任何attribute value，要刪掉此empty tuple
					if(t->tuples[tupleIndex].CheckEmpty()){
						t->tuples.pop_back();
					}
				}
			}
		}
	}
	return true;
}

//---------------------------------------
// bool SELECT_InsertTuplesWithWhere(Table*, SelectInst*, vector<Table*>, vector<int>*)
//		Insert tuples by SELECT with WHERE.
//---------------------------------------
bool TableSet::SELECT_InsertTuplesWithWhere(Table* t, SelectInst* sinst, vector<Table*> selectedTables, vector<int>* TIndex)
{
	if (selectedTables.size() == 1){
		
		if(!CheckWhereValid(sinst, selectedTables)){
			return false;
		}
		
		for(int i = 0 ; i < (int)selectedTables[0]->tuples.size() ; i++){
			
			if(CheckWhereCondition(sinst, selectedTables, i, -1)){
				int tupleIndex = t->InsertEmptyTuple();
				
				if(sinst->isSelectAllAttrs[0]){
					if(!t->CopyValuesToTuple(selectedTables[0], tupleIndex, i))
						return false;
				}
				else {
					
					for(int j = 0 ; j < (int)sinst->selectedAttributesNames.size() ; j++){
						//此attribute屬於此table
						if(TIndex->at(j) == 0){
							if(!t->CopyValueToTuple(selectedTables[0], sinst->selectedAttributesNames[j], tupleIndex, i))
								return false;
						}
					}
				}
				
				//如果沒有加入任何attribute value，要刪掉此empty tuple
				if(t->tuples[tupleIndex].CheckEmpty()){
					t->tuples.pop_back();
				}
			}
		}
	} else if (selectedTables.size() == 2){
		
		if(!CheckWhereValid(sinst, selectedTables)){
			return false;
		}
		
		if(selectedTables[0]->tuples.size() == 0){
			vector<Table*> tempTables;
			tempTables.push_back(selectedTables[1]);
			if(!SELECT_InsertTuples(t, sinst, tempTables, TIndex))
				return false;
		} else if(selectedTables[1]->tuples.size() == 0){
			vector<Table*> tempTables;
			tempTables.push_back(selectedTables[0]);
			if(!SELECT_InsertTuples(t, sinst, tempTables, TIndex))
				return false;
		} else{
			
			for(int i = 0 ; i < (int)selectedTables[0]->tuples.size() ; i++){
				for(int j = 0 ; j < (int)selectedTables[1]->tuples.size() ; j++){
					
					if(CheckWhereCondition(sinst, selectedTables, i, j)){
						int tupleIndex = t->InsertEmptyTuple();
						
						if(sinst->isSelectAllAttrs[0] || sinst->isSelectAllAttrs[1]){
							if(sinst->isSelectAllAttrs[0]){
								if(!t->CopyValuesToTuple(selectedTables[0], tupleIndex, i))
									return false;
							}
							if(sinst->isSelectAllAttrs[1]){
								if(!t->CopyValuesToTuple(selectedTables[1], tupleIndex, j))
									return false;
							}
						}
						else {
							for(int k = 0 ; k < (int)sinst->selectedAttributesNames.size() ; k++){
								//此attribute屬於First table
								if(TIndex->at(k) == 0){
									if(!t->CopyValueToTuple(selectedTables[0], sinst->selectedAttributesNames[k], tupleIndex, i))
										return false;
								}
								//此attribute屬於Second table
								if(TIndex->at(k) == 1){
									if(!t->CopyValueToTuple(selectedTables[1], sinst->selectedAttributesNames[k], tupleIndex, j))
										return false;
								}
							}
						}
						
						//如果沒有加入任何attribute value，要刪掉此empty tuple
						if(t->tuples[tupleIndex].CheckEmpty()){
							t->tuples.pop_back();
						}
					}
				}
			}
		}
	}
	return true;
}

//-------------------------------------------
// bool CheckWhereValid(SelectInst*, vector<Table*>)
//		Check the WHERE is valid or not.
//-------------------------------------------
bool TableSet::CheckWhereValid(SelectInst* sinst, vector<Table*> selectedTables)
{
	//檢查各table name是否包括在selectedTables
	for(int i = 0 ; i < (int)sinst->WHERE_FirstAttrTables.size() ; i++){
		if(sinst->WHERE_FirstAttrTables[i].compare("") != 0){
			bool flag = false;
			string n1 = sinst->WHERE_FirstAttrTables[i];
			transform(n1.begin(), n1.end(), n1.begin(),::tolower);
			for(int j = 0 ; j < (int)selectedTables.size() ; j++){
				string n2 = selectedTables[j]->getTableName();
				transform(n2.begin(), n2.end(), n2.begin(),::tolower);
				
				if(n1.compare(n2) == 0){
					flag = true;
					break;
				}
			}
			if(!flag){
				cout << "- Error: The table " << sinst->WHERE_FirstAttrTables[i] <<
					" cannot be find or is empty.\n";
			}
		}
	}
	for(int i = 0 ; i < (int)sinst->WHERE_SecondAttrTables.size() ; i++){
		if(sinst->WHERE_SecondAttrTables[i].compare("") != 0){
			bool flag = false;
			string n1 = sinst->WHERE_SecondAttrTables[i];
			transform(n1.begin(), n1.end(), n1.begin(),::tolower);
			for(int j = 0 ; j < (int)selectedTables.size() ; j++){
				string n2 = selectedTables[j]->getTableName();
				transform(n2.begin(), n2.end(), n2.begin(),::tolower);
				
				if(n1.compare(n2) == 0){
					flag = true;
					break;
				}
			}
			if(!flag){
				cout << "- Error: The table " << sinst->WHERE_SecondAttrTables[i] <<
					" cannot be find or is empty.\n";
			}
		}
	}
	
	//檢查attribute是否都出現在FROM的tables, 以及是否都沒出現
	for(int i = 0 ; i < (int)sinst->WHERE_FirstAttrNames.size() ; i++){
		if(sinst->WHERE_FirstAttrNames[i].compare("NULL") == 0)
			continue;
		if(sinst->WHERE_FirstTypes[i] == 2){
			bool flags [2] = {false, false};
			for(int j = 0 ; j < (int)selectedTables.size() ; j++){
				if(selectedTables[j]->ContainAttribute(sinst->WHERE_FirstAttrNames[i])){
					flags[j] = true;
				}
			}
			
			//都出現且table等於2且此attrbute有table name
			if(flags[0] && flags[1] && selectedTables.size() == 2 && sinst->WHERE_FirstAttrTables[i].compare("") == 0){
				cout << "- Error: The attribute " << sinst->WHERE_FirstAttrNames[i] <<
					" is an ambiguous attribute between two tables, but not used as a prefix in the attribute.\n";
				return false;
			}
			
			//都沒出現
			if(!flags[0] && !flags[1]){
				cout << "- Error: The attribute " << sinst->WHERE_FirstAttrNames[i] <<
					" doesn't exist in all tables.\n";
				return false;
			}
		}
	}
	for(int i = 0 ; i < (int)sinst->WHERE_SecondAttrNames.size() ; i++){
		if(sinst->WHERE_FirstAttrNames[i].compare("NULL") == 0)
			continue;
		if(sinst->WHERE_SecondTypes[i] == 2){
			bool flags [2] = {false, false};
			for(int j = 0 ; j < (int)selectedTables.size() ; j++){
				if(selectedTables[j]->ContainAttribute(sinst->WHERE_SecondAttrNames[i])){
					flags[j] = true;
				}
			}
			
			//都出現且table等於2且此attrbute有table name
			if(flags[0] && flags[1] && selectedTables.size() == 2 && sinst->WHERE_SecondAttrTables[i].compare("") == 0){
				cout << "- Error: The attribute " << sinst->WHERE_SecondAttrNames[i] <<
					" is an ambiguous attribute between two tables, but not used as a prefix in the attribute.\n";
				return false;
			}
			
			//都沒出現
			if(!flags[0] && !flags[1]){
				cout << "- Error: The attribute " << sinst->WHERE_SecondAttrNames[i] <<
					" doesn't exist in all tables.\n";
				return false;
			}
		}
	}
	return true;
}

//-------------------------------------------
// bool CheckWhereCondition(SelectInst*, vector<Table*>, int, int)
//		Check the WHERE condition is true or false.
//-------------------------------------------
bool TableSet::CheckWhereCondition(SelectInst* sinst, vector<Table*> selectedTables, int tplIdx1, int tplIdx2)
{
	//檢查條件
	if(tplIdx2 == -1){
		
		bool cond[2] = {false, false};
		
		for(int i = 0 ; i < (int)sinst->WHERE_FirstAttrNames.size() ; i++){
			int valueFirstInt;
			string* valueFirstStr;
			int valueFirstType;	//0 = int, 1 = varchar, -1 = NULL
			
			if(sinst->WHERE_FirstAttrNames[i].compare("NULL") == 0){
				valueFirstType = -1;
			}
			else {
				switch(sinst->WHERE_FirstTypes[i]){
					case 0:{
						/*Integer*/
						valueFirstInt = atoi(sinst->WHERE_FirstAttrNames[i].c_str());
						valueFirstType = 0;
						break;
					}
					case 1:{
						/*VarChar*/
						valueFirstStr = new string(sinst->WHERE_FirstAttrNames[i]);
						valueFirstType = 1;
						break;
					}
					case 2:{
						/*Attribute*/
						if(!selectedTables[0]->ContainAttribute(sinst->WHERE_FirstAttrNames[i])) {
							return false;
						}
						
						string *sValue = selectedTables[0]->tuples[tplIdx1].getValue(sinst->WHERE_FirstAttrNames[i]);
						//如果value是NULL
						if(sValue == NULL){
							valueFirstType = -1;
							break;
						}
						switch(selectedTables[0]->GetAttributeType(sinst->WHERE_FirstAttrNames[i])){
							case 0:
								valueFirstInt = atoi((*sValue).c_str());
								valueFirstType = 0;
								break;
							case 1:
								valueFirstStr = sValue;
								valueFirstType = 1;
								break;
						}
						break;
					}
					case -1:{
						cout << "- Error: WHERE left item has to exist.\n";
						return false;
						break;
					}
				}
			}
			
			//如果右值不存在且左值為NULL
			if(sinst->WHERE_SecondTypes[i] == -1 && valueFirstType == -1){
				cout << "- Warning: There is only one value in " << (i+1) << 
					" condition, and it's NULL!\n";
				cond[i] = false;
				continue;
			}
			//如果右值不存在且左值為int且為0
			else if(sinst->WHERE_SecondTypes[i] == -1 && valueFirstType == 0 && valueFirstInt == 0){
				cout << "- Warning: There is only one value in " << (i+1) << 
					" condition, and it's 0!\n";
				cond[i] = false;
				continue;
			}
			//如果右值不存在且左值不為NULL && 不是int且為0
			else if(sinst->WHERE_SecondTypes[i] == -1){
				cout << "- Warning: There is only one value in " << (i+1) << 
					" condition, and it's not NULL!\n";
				cond[i] = true;
				continue;
			}
			
			int valueSecondInt;
			string* valueSecondStr;
			int valueSecondType;	//0 = int, 1 = varchar, -1 = NULL
			
			if(sinst->WHERE_SecondAttrNames[i].compare("NULL") == 0){
				valueSecondType = -1;
			}
			else {
				switch(sinst->WHERE_SecondTypes[i]){
					case 0:{
						/*Integer*/
						valueSecondInt = atoi(sinst->WHERE_SecondAttrNames[i].c_str());
						valueSecondType = 0;
						break;
					}
					case 1:{
						/*VarChar*/
						valueSecondStr = new string(sinst->WHERE_SecondAttrNames[i]);
						valueSecondType = 1;
						break;
					}
					case 2:{
						/*Attribute*/
						if(!selectedTables[0]->ContainAttribute(sinst->WHERE_SecondAttrNames[i]))
							return false;
						
						string *sValue = selectedTables[0]->tuples[tplIdx1].getValue(sinst->WHERE_SecondAttrNames[i]);
						//如果value是NULL
						if(sValue == NULL){
							valueSecondType = -1;
							break;
						}
						switch(selectedTables[0]->GetAttributeType(sinst->WHERE_SecondAttrNames[i])){
							case 0:
								valueSecondInt = atoi((*sValue).c_str());
								valueSecondType = 0;
								break;
							case 1:
								valueSecondStr = sValue;
								valueSecondType = 1;
								break;
						}
						break;
					}
				}
			}
			
			//不同類型，int != char, 或是其中一方為NULL
			if(valueFirstType != valueSecondType){
				cout << "- Warning: The two types cannot be matched in " << (i+1) << 
					" condition.\n";
				cond[i] = false;
				continue;
			}
			
			switch(valueFirstType){
				case 0:{
					/*Integer*/
					switch(sinst->WHERE_ExprTypes[i]){
						case 0:
							if(valueFirstInt == valueSecondInt){
								cond[i] = true;
							} else cond[i] = false;
							break;
						case 1:
							if(valueFirstInt != valueSecondInt){
								cond[i] = true;
							} else cond[i] = false;
							break;
						case 2:
							if(valueFirstInt < valueSecondInt){
								cond[i] = true;
							} else cond[i] = false;
							break;
						case 3:
							if(valueFirstInt > valueSecondInt){
								cond[i] = true;
							} else cond[i] = false;
							break;
					}
					break;
				}
				case 1:{
					/*String*/
					switch(sinst->WHERE_ExprTypes[i]){
						case 0:
							if((*valueFirstStr).compare(*(valueSecondStr)) == 0){
								cond[i] = true;
							} else cond[i] = false;
							break;
						case 1:
							if((*valueFirstStr).compare(*(valueSecondStr)) != 0){
								cond[i] = true;
							} else cond[i] = false;
							break;
						default:
							cout << "- Warning: The type varchar cannot be bigger or smaller than another.\n";
							cond[i] = false;
							break;
					}
					break;
				}
				case -1:{
					/*NULL*/
					if(sinst->WHERE_ExprTypes[i] == 0) cond[i] = true;
					else cond[i] = false;
					break;
				}
			}
		}
		
		bool flag = false;
		for(int i = 0 ; i < 2 ; i++){
			switch(sinst->WHERE_ExprAO){
				case 0:
					/*AND*/
					if(!cond[i]){
						return false;
					} else flag = true;
					break;
				case 1:
					/*OR*/
					if(cond[i]){
						return true;
					} else flag = false;
					break;
				case -1:
					/*Only one condition*/
					return cond[0];
					break;
			}
		}
		
		return flag;
		
	}
	else {
		
		bool cond[2] = {false, false};
		
		for(int i = 0 ; i < (int)sinst->WHERE_FirstAttrNames.size() ; i++){
			int valueFirstInt;
			string* valueFirstStr;
			int valueFirstType;	//0 = int, 1 = varchar, -1 = NULL
			int FirstTIndex = -1;
			
			if(sinst->WHERE_FirstAttrNames[i].compare("NULL") == 0){
			//if(sinst->WHERE_FirstAttrNames[i].compare("") == 0){
				valueFirstType = -1;
			}
			else {
				switch(sinst->WHERE_FirstTypes[i]){
					case 0:{
						/*Integer*/
						valueFirstInt = atoi(sinst->WHERE_FirstAttrNames[i].c_str());
						valueFirstType = 0;
						break;
					}
					case 1:{
						/*VarChar*/
						valueFirstStr = new string(sinst->WHERE_FirstAttrNames[i]);
						valueFirstType = 1;
						break;
					}
					case 2:{
						/*Attribute*/
						for(int j = 0 ; j < (int)selectedTables.size() ; j++){
							//沒有table
							if(sinst->WHERE_FirstAttrTables[i].compare("") == 0){
								if(selectedTables[j]->ContainAttribute(sinst->WHERE_FirstAttrNames[i])){
									FirstTIndex = j;
									break;
								}
							}
							//有table
							else {
								string n1 = sinst->WHERE_FirstAttrTables[i];
								transform(n1.begin(), n1.end(), n1.begin(),::tolower);
								string n2 = selectedTables[j]->getTableName();
								transform(n2.begin(), n2.end(), n2.begin(),::tolower);
								if(n1.compare(n2) == 0 && selectedTables[j]->ContainAttribute(sinst->WHERE_FirstAttrNames[i])){
									FirstTIndex = j;
									break;
								}
							}
						}
						
						string *sValue = new string();
						if(FirstTIndex == -1){
							cout << "- Error: Cannot find attribute " << sinst->WHERE_FirstAttrNames[i] <<
								".\n";
							return false;
						} else if(FirstTIndex == 0){
							sValue = selectedTables[FirstTIndex]->tuples[tplIdx1].getValue(sinst->WHERE_FirstAttrNames[i]);
						} else if(FirstTIndex == 1){
							sValue = selectedTables[FirstTIndex]->tuples[tplIdx2].getValue(sinst->WHERE_FirstAttrNames[i]);
						}
						
						//如果value是NULL
						if(sValue == NULL){
							valueFirstType = -1;
							break;
						}
						switch(selectedTables[FirstTIndex]->GetAttributeType(sinst->WHERE_FirstAttrNames[i])){
							case 0:
								valueFirstInt = atoi((*sValue).c_str());
								valueFirstType = 0;
								break;
							case 1:
								valueFirstStr = sValue;
								valueFirstType = 1;
								break;
						}
						break;
					}
					case -1:{
						cout << "- Error: WHERE left item has to exist.\n";
						return false;
						break;
					}
				}
			}
			
			//如果右值不存在且左值為NULL
			if(sinst->WHERE_SecondTypes[i] == -1 && valueFirstType == -1){
				cout << "- Warning: There is only one value in " << (i+1) << 
					" condition, and it's NULL!\n";
				cond[i] = false;
				continue;
			}
			//如果右值不存在且左值為int且為0
			else if(sinst->WHERE_SecondTypes[i] == -1 && valueFirstType == 0 && valueFirstInt == 0){
				cout << "- Warning: There is only one value in " << (i+1) << 
					" condition, and it's 0!\n";
				cond[i] = false;
				continue;
			}
			//如果右值不存在且左值不為NULL && 不是int且為0
			else if(sinst->WHERE_SecondTypes[i] == -1){
				cout << "- Warning: There is only one value in " << (i+1) << 
					" condition, and it's not NULL!\n";
				cond[i] = true;
				continue;
			}
			
			int valueSecondInt;
			string* valueSecondStr;
			int valueSecondType;	//0 = int, 1 = varchar, -1 = NULL
			int SecondTIndex = -1;
			
			if(sinst->WHERE_SecondAttrNames[i].compare("NULL") == 0){
			//if(sinst->WHERE_SecondAttrNames[i].compare("") == 0){
				valueSecondType = -1;
			}
			else {
				switch(sinst->WHERE_SecondTypes[i]){
					case 0:{
						/*Integer*/
						valueSecondInt = atoi(sinst->WHERE_SecondAttrNames[i].c_str());
						valueSecondType = 0;
						break;
					}
					case 1:{
						/*VarChar*/
						valueSecondStr = new string(sinst->WHERE_SecondAttrNames[i]);
						valueSecondType = 1;
						break;
					}
					case 2:{
						/*Attribute*/
						for(int j = 0 ; j < (int)selectedTables.size() ; j++){
							//沒有table
							if(sinst->WHERE_SecondAttrTables[i].compare("") == 0){
								if(selectedTables[j]->ContainAttribute(sinst->WHERE_SecondAttrNames[i])){
									SecondTIndex = j;
									break;
								}
							}
							//有table
							else {
								string n1 = sinst->WHERE_SecondAttrTables[i];
								transform(n1.begin(), n1.end(), n1.begin(),::tolower);
								string n2 = selectedTables[j]->getTableName();
								transform(n2.begin(), n2.end(), n2.begin(),::tolower);
								if(n1.compare(n2) == 0 && selectedTables[j]->ContainAttribute(sinst->WHERE_SecondAttrNames[i])){
									SecondTIndex = j;
									break;
								}
							}
						}
						
						string *sValue = new string();
						if(SecondTIndex == -1){
							cout << "- Error: Cannot find attribute " << sinst->WHERE_SecondAttrNames[i] <<
								".\n";
							return false;
						} else if(SecondTIndex == 0){
							sValue = selectedTables[SecondTIndex]->tuples[tplIdx1].getValue(sinst->WHERE_SecondAttrNames[i]);
						} else if(SecondTIndex == 1){
							sValue = selectedTables[SecondTIndex]->tuples[tplIdx2].getValue(sinst->WHERE_SecondAttrNames[i]);
						}
						
						//如果value是NULL
						if(sValue == NULL){
							valueSecondType = -1;
							break;
						}
						switch(selectedTables[SecondTIndex]->GetAttributeType(sinst->WHERE_SecondAttrNames[i])){
							case 0:
								valueSecondInt = atoi((*sValue).c_str());
								valueSecondType = 0;
								break;
							case 1:
								valueSecondStr = sValue;
								valueSecondType = 1;
								break;
						}
						break;
					}
				}
			}
			
			//不同類型，int != char, 或是其中一方為NULL
			if(valueFirstType != valueSecondType){
				cout << "- Warning: The two types cannot be matched in " << (i+1) << 
					" condition.\n";
				cond[i] = false;
				continue;
			}
			
			switch(valueFirstType){
				case 0:{
					/*Integer*/
					switch(sinst->WHERE_ExprTypes[i]){
						case 0:
							if(valueFirstInt == valueSecondInt){
								cond[i] = true;
							} else cond[i] = false;
							break;
						case 1:
							if(valueFirstInt != valueSecondInt){
								cond[i] = true;
							} else cond[i] = false;
							break;
						case 2:
							if(valueFirstInt < valueSecondInt){
								cond[i] = true;
							} else cond[i] = false;
							break;
						case 3:
							if(valueFirstInt > valueSecondInt){
								cond[i] = true;
							} else cond[i] = false;
							break;
					}
					break;
				}
				case 1:{
					/*String*/
					switch(sinst->WHERE_ExprTypes[i]){
						case 0:
							if((*valueFirstStr).compare(*(valueSecondStr)) == 0){
								cond[i] = true;
							} else cond[i] = false;
							break;
						case 1:
							if((*valueFirstStr).compare(*(valueSecondStr)) != 0){
								cond[i] = true;
							} else cond[i] = false;
							break;
						default:
							cout << "- Warning: The type varchar cannot be bigger or smaller than another.\n";
							cond[i] = false;
							break;
					}
					break;
				}
				case -1:{
					/*NULL*/
					if(sinst->WHERE_ExprTypes[i] == 0) cond[i] = true;
					else cond[i] = false;
					break;
				}
			}
		}
		
		bool flag = false;
		for(int i = 0 ; i < 2 ; i++){
			switch(sinst->WHERE_ExprAO){
				case 0:
					/*AND*/
					if(!cond[i]){
						return false;
					} else flag = true;
					break;
				case 1:
					/*OR*/
					if(cond[i]){
						return true;
					} else flag = false;
					break;
				case -1:
					/*Only one condition*/
					return cond[0];
					break;
			}
		}
		
		return flag;
		
	}
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

//-----------------------------------------------
// a function to store table information in disk
//-----------------------------------------------
void TableSet::InformationWrite_TableSet () 
{
	// TableSetIndexFile
	ofstream* tablesetInformation= new ofstream();
	tablesetInformation->open (TableSetIndexFile, ios::out);
	
	for (int i=0; i<(int)tableVector.size(); i++) {
		*tablesetInformation << tableVector[i].returnTableFileName () << endl;

		tableVector[i].InformationWrite_Table ();
	}

	tablesetInformation->close();
}

void TableSet::InformationRead_TableSet () 
{
	fstream fp;
	string inputString;
	fp.open (TableSetIndexFile, ios::in);
	if (!fp) {
		cout << "- No Existing TableSet\n- Initializing..." << endl;
		return;
	}
	//cout << "- Loading Existing TableSet..." << endl;
	while (getline (fp, inputString)) {
		Table* t = new Table();
		t->InformationRead_Table(inputString);
		PushTable(*t);
	}
	fp.close();
	//cout << "- Start Database" << endl;
}

void TableSet::LoadTableSet ()
{
	for (int i=0; i<(int)tableVector.size(); i++) {
		tableVector[i].LoadTable ();
	}
	//cout << "- Finish Loading Table" << endl;
}