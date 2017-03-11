#include <iostream>
#include "Table.h"

//--------------
// Table(CreateInst)
//		Create a table and set attributes.
//--------------
Table::Table(CreateInst *cinst)
{
	tableName = cinst->tableName;
	
	for(int i = 0 ; i < cinst->attributeNum ; i++){
		Attribute a;
		a.name = cinst->attributeNames[i];
		a.type = cinst->attributeTypes[i];
		a.isPK = cinst->isPK[i];
		if(a.isPK) 
			PKIndexes.push_back(i);
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
void Table::Tuple::setValue(int index, string value)
{
	values[index].value = value;
}

//--------------
// void setValue(string name, string value)
//		Set value of tuple by attribute name.
//		(INSERT with attribute name)
//--------------
void Table::Tuple::setValue(string name, string value)
{
	for(vector<Attribute>::iterator it = values.begin(); it != values.end(); it++){
		if(it->name.compare(name) == 0)
			it->value = value;
	}
}

//--------------
// string getValue(string name)
//		Get value by attribute name.
//--------------
string Table::Tuple::getValue(string name)
{
	for(vector<Attribute>::iterator it = values.begin(); it != values.end(); it++){
		if(it->name.compare(name) == 0)
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
	//TODO: check duplicate PK value.
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
	
	if(!iinst->isWithName){
		
		//Check attribute value number of instruction without attribute name.
		if((int)attributes.size() != iinst->attributeValueNum)
			return false;
		
		//Check attribute type of instruction without attribute name.
		
	} else {
		
		//Check attribute name of instruction with attribute name.
		
		//Check attribute type of instruction with attribute name.
		
	}
	
	
	
	//Check duplicate PK.
	for (int i = 0 ; i < (int)tuples.size() ; i++){
		for(int j = 0 ; j < (int)PKIndexes.size() ; j++){
			if(tuples[i].values[j].value.compare(iinst->attributeValues[j]) == 0)
				return false;
		}
	}
	
	return true;
}

string Table::getTableName()
{
	return tableName;
}

