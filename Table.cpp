#include <iostream>
#include "Table.h"

//--------------
// Table(CreateInst)
//		Create a table and set attributes.
//--------------
Table::Table(CreateInst cinst)
{
	tableName = cinst.tableName;
	
	for(int i = 0 ; i < cinst.attributeNames.size ; i++){
		Attribute a;
		a.name = cinst.attributeNames[i];
		a.type = cinst.attributeTypes[i];
		a.isPK = cinst.isPK[i];
		
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
	for(vector<Attribute>::iterator it = values.begin() : values.end()){
		if(it.name.compare(name) == 0)
			it.value = value;
	}
}

//--------------
// string getValue(string name)
//		Get value by attribute name.
//--------------
string Table::Tuple::getValue(string name)
{
	for(vector<Attribute>::iterator it = values.begin() : values.end()){
		if(it.name.compare(name) == 0)
			return it.value;
	}
}

//--------------
// void InsertTuple(InsertInst)
//		Generate a tuple with attribute values.
//--------------
void Table::InsertTuple(InsertInst iinst)
{
	Tuple t;
	
	t.values = attributes;
	//TODO: check duplicate PK value.
}

//--------------
// bool CheckInsertInst(InsertInst)
//		Check this INSERT is valid or not.
//		Like duplicate PK, or different attributes number of
//	instruction without attribute name.
//--------------
bool CheckInsertInst(InsertInst)
{
	return true;
}

