#include <iostream>
#include <algorithm>
#include "Table.h"

//--------------
// Table(CreateInst)
//		Create a table and set attributes.
//--------------
Table::Table(CreateInst *cinst)
{
	tableName = cinst->tableName;
	
	isHidedPK = true;
	for(int i = 0 ; i < (int)cinst->attributeNames.size() ; i++){
		Attribute a;
		a.name = cinst->attributeNames[i];
		a.type = cinst->attributeTypes[i];
		a.isPK = cinst->isPK[i];
		if (a.isPK){
			PKIndexes.push_back(i);
			isHidedPK = false;
		}
		a.varCharSize = cinst->varCharSizes[i];
		
		attributes.push_back(a);
	}
}

Table::~Table()
{
	attributes.clear();
	tuples.clear();
}

//--------------
// void setValue(int index, string value)
//		Set value of tuple by index number.
//		(INSERT without attribute name)
//--------------
void Table::Tuple::setValue(int index, string* value)
{
	string *s = new string(*value);
	values[index].value = s;
}

//--------------
// void setValue(string name, string value)
//		Set value of tuple by attribute name.
//		(INSERT with attribute name)
//--------------
void Table::Tuple::setValue(string name, string* value)
{
	string n1 = name;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for(vector<Attribute>::iterator it = values.begin(); it != values.end(); it++){
		string n2 = it->name;
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0){
			string *s = new string(*value);
			it->value = s;
			return;
		}
	} 
	
	cout << "Error: Cannot find this attribute name." << endl;
}

//--------------
// string getValue(string name)
//		Get value by attribute name.
//--------------
string* Table::Tuple::getValue(string name)
{
	string n1 = name;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for(vector<Attribute>::iterator it = values.begin(); it != values.end(); it++){
		string n2 = it->name;
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0)
			return it->value;
	}
	
	cout << "Error: Cannot find this attribute name." << endl;
	return NULL;
}

//--------------
// void InsertTuple(InsertInst)
//		Generate a tuple with attribute values.
//--------------
void Table::InsertTuple(InsertInst *iinst)
{
	Tuple t;
	t.values = attributes;
	t.isHidedPK = isHidedPK;
	for (int i = 0 ; i < (int)attributes.size() ; i++){
		t.values[i].value = NULL;
	}
	
	if (!iinst->isWithName){
		for (int i = 0 ; i < (int)iinst->insertedValues.size() ; i++){
			string *value = iinst->insertedValues[i];
			t.setValue(i, value);
		}
	} else {
		for (int i = 0 ; i < (int)iinst->insertedAttributes.size() ; i++){
			string name = iinst->insertedAttributes[i];
			string *value = iinst->insertedValues[i];
			t.setValue(name, value);
		}
	}
	
	//Set hided PK
	if (isHidedPK){
		string s = "";
		for (int i = 0 ; i < (int)attributes.size() ; i++){
			if(t.values[i].value != NULL) {
				s += *(t.values[i].value);
			}
		}
		t.hidedPK = new string(s);
	}
	
	tuples.push_back(t);
}

//--------------
// bool CheckInsertInst(InsertInst)
//		Check this INSERT is valid or not.
//		Like duplicate PK, or different attributes number of
//	instruction without attribute name, or different type of
//	attribute of instruction, or different attribute names 
//	of instruction with attribute names.
//--------------
bool Table::CheckInsertInst(InsertInst *iinst)
{
	if(iinst->insertedAttributes.size() != iinst->insertedValues.size()){
		cout << "Error: INSERT attribute name number doesn't match to value number\n";
		return false;
	}
	
	if(!iinst->isWithName){
		
		//Check attribute value number of instruction without attribute name.
		if ((int)attributes.size() != (int)iinst->insertedValues.size()){
			cout << "Error: INSERT value number doesn't match to table attribute number.\n";
			return false;
		}
		
		//Check attribute type of instruction without attribute name.
		for (int i = 0 ; i < (int)attributes.size() ; i++){
			if(attributes[i].type != iinst->insertedValueTypes[i]){
				cout << "Error: INSERT value type doesn't match to table attribute type.\n";
				return false;
			}
		}
		
		//Check NULL value of PK.
		for (int i = 0 ; i < (int)PKIndexes.size() ; i++){
			if (iinst->insertedValues[i] == NULL){
				cout << "Error: There exists NULL value in PK.\n";
				return false;
			}
		}
		
		//Check duplicate PK without attribute name.
		string s = "";
		if (isHidedPK){
			for (int i = 0 ; i < (int)iinst->insertedValues.size() ; i++){
				if(iinst->insertedValues[i] != NULL)
					s += *(iinst->insertedValues[i]);
			}
		}
		for (int i = 0 ; i < (int)tuples.size() ; i++){
			if (isHidedPK) {
				if (tuples[i].hidedPK != NULL && (*(tuples[i].hidedPK)).compare(s) == 0){
					cout << "Error: There exists duplicate tuple.\n";
					return false;
				}
			} else {
				for(int j = 0 ; j < (int)PKIndexes.size() ; j++){
					if((*(tuples[i].values[j].value)).compare(*(iinst->insertedValues[j])) == 0){
						cout << "Error: There exists duplicate PK value.\n";
						return false;
					}
				}
			}
		}
		
		//Check varchar size.
		for (int i = 0 ; i < (int)attributes.size() ; i++){
			if(attributes[i].type == 1 && 
				((int)(*(iinst->insertedValues[i])).size() > attributes[i].varCharSize)){
					cout << "Error: Different varchar size with table.\n";
					return false;
				}
		}
		
	} else {
		
		//Check attribute name and type of instruction with attribute name.
		//Check varchar size.
		bool nameChecker;
		for (int i = 0 ; i < (int)iinst->insertedAttributes.size() ; i++){
			nameChecker = false;
			string n1 = iinst->insertedAttributes[i];
			transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
			for (int j = 0 ; j < (int)attributes.size() ; j++){
				string n2 = attributes[j].name;
				transform(n2.begin(), n2.end(), n2.begin(), ::tolower);
				if (n1.compare(n2) == 0){
					//For name
					nameChecker = true;	
					//For type
					if (attributes[j].type != iinst->insertedValueTypes[i]){
						cout << "Error: INSERT value type doesn't match to table attribute type.\n";
						return false;
					}
					//For varchar size
					if(attributes[j].type == 1 && 
						((int)((*(iinst->insertedValues[i])).size()) > attributes[j].varCharSize)){
							cout << "Error: Different varchar size with table.\n";
							return false;
						}
				}
			}
			if(nameChecker == false){
				cout << "Error: INSERT name doesn't match to table attribute name.\n";
				return false;
			}
		}
		
		//Check NULL value and duplicate value of PK.
		//Check duplicate PK without attribute name.
		if (!isHidedPK){
			bool nullPKChecker;
			for (int i = 0 ; i < (int)PKIndexes.size() ; i++){
				nullPKChecker = false;
				string n1 = attributes[i].name;
				transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
				for (int j = 0 ; j < (int)iinst->insertedAttributes.size() ; j++){
					string n2 = iinst->insertedAttributes[i];
					transform(n2.begin(), n2.end(), n2.begin(), ::tolower);
					if(n1.compare(n2) == 0){
						//For NULL
						nullPKChecker = true;
						//For duplicate
						for (int k = 0 ; k < (int)tuples.size() ; k++){
							if((*(tuples[k].values[i].value)).compare(*(iinst->insertedValues[j])) == 0){
								cout << "Error: There exists duplicate PK value.\n";
								return false;
							}
						}
					}
				}
				if(nullPKChecker == false){
					cout << "Error: There exists NULL value in PK.\n";
					return false;
				}
			}
		} else {
			string s = "";
			for (int i = 0 ; i < (int)iinst->insertedValues.size() ; i++){
				if(iinst->insertedValues[i] != NULL)
					s += *(iinst->insertedValues[i]);
			}
			
			for (int i = 0 ; i < (int)tuples.size() ; i++){
				if (tuples[i].hidedPK != NULL && (*(tuples[i].hidedPK)).compare(s) == 0){
					cout << "Error: There exists duplicate tuple.\n";
					return false;
				}
			}
		}
		
	}
	
	return true;
}

string Table::getTableName()
{
	return tableName;
}

void Table::ShowTable()
{
	vector<int> printSize;
	
	//Print out table name
	cout << tableName << endl;
	
	//Print out attribute names
	for (int i = 0 ; i < (int)attributes.size() ; i++){
		//Type = int, 11 size char, 2 space
		if(attributes[i].type == 0){
			int maxLength;
			if((int)attributes[i].name.size() >= 11){
				maxLength = (int)attributes[i].name.size();
			} else maxLength = 11;
			printSize.push_back(maxLength);
			
			cout << " ";
			for(int j = 0 ; j < (maxLength - (int)attributes[i].name.size()) ; j++){
				cout << " ";
			}
			cout << attributes[i].name << " ";
		} 
		//Type = varchar, varCharSize size char, 2space
		else if (attributes[i].type == 1){
			int maxLength;
			if((int)attributes[i].name.size() >= attributes[i].varCharSize){
				maxLength = (int)attributes[i].name.size();
			} else maxLength = attributes[i].varCharSize;
			printSize.push_back(maxLength);
			
			cout << " ";
			for(int j = 0 ; j < (maxLength - (int)attributes[i].name.size()) ; j++){
				cout << " ";
			}
			cout << attributes[i].name << " ";
		}
	}
	cout << endl;
	
	//Print out tuples
	for (int i = 0 ; i < (int)tuples.size() ; i++){
		for (int j = 0 ; j < (int)tuples[i].values.size() ; j++){
			cout << " ";
			for (int k = 0 ; k < (printSize[j] - (int)((*(tuples[i].values[j].value)).size())) ; k++){
				cout << " ";
			}
			cout << *(tuples[i].values[j].value) << " ";
		}
		cout << endl;
	}
	
}