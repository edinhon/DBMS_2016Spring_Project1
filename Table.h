#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>
#include <vector>
#include "Instruction.h"
#include "CreateInst.h"
#include "InsertInst.h"
#include "SelectInst.h"

using namespace std;

class Table {
	public:
		
		class Attribute {
			public:
				string name;
				int type;	// 0 = int, 1 = varchar
				int varCharSize; // size of varchar
				string *value;
				bool isPK;
				Attribute& operator= (Attribute const &that){
					if (this != &that) {
						name = that.name;
						type = that.type;
						varCharSize = that.varCharSize;
						value = that.value;
						isPK = that.isPK;
					}
					return *this;
				}
		};
		
		class Tuple {
			public:
				vector<Attribute> values;
				
				//--------------
				// void setValue(int index, string* value)
				//		Set value of tuple by index number.
				//		(INSERT without attribute name)
				//--------------
				void setValue(int index, string* value);
				
				//--------------
				// void setValue(string name, string value)
				//		Set value of tuple by attribute name.
				//		(INSERT with attribute name)
				//--------------
				void setValue(string name, string* value);
				
				//--------------
				// string getValue(string name)
				//		Get value by attribute name.
				//--------------
				string* getValue(string name);
				
				string* hidedPK;
				
				bool isHidedPK;
		};
		
		//--------------
		// Table(CreateInst)
		//		Create a table and set attributes.
		//--------------
		Table(CreateInst*);
		
		//---------------------------------------------------
		// Table()
		//		Create a empty table. Used in SELECT instruction.
		//---------------------------------------------------
		Table();
		
		~Table();
		
		//--------------
		// void InsertTuple(InsertInst)
		//		Generate a tuple with attribute values.
		//--------------
		void InsertTuple(InsertInst*);
		
		//--------------
		// bool CheckInsertInst(InsertInst)
		//		Check this INSERT is valid or not.
		//		Like duplicate PK, or different attributes number of
		//	instruction without attribute name, or different type of
		//	attribute of instruction, or different attribute names 
		//	of instruction with attribute names.
		//--------------
		bool CheckInsertInst(InsertInst*);

		//-------------------------------------------------
		// bool CopyAttribute(Table*, string)
		//		Copy a new attribute column into a empty Table.
		//-------------------------------------------------
		bool CopyAttribute(Table*, string);
		
		//-------------------------------------------------
		// bool CopyAttributes(Table*)
		//		Copy all attribute columns of input Table into a empty Table.
		//-------------------------------------------------
		bool CopyAttributes(Table*);
		
		//-------------------------------------------------
		// bool ContainAttribute(string)
		//		Check the attribute by name in table or not.
		//-------------------------------------------------
		bool ContainAttribute(string);
		/*
		//-------------------------------------------------
		// Attribute GetAttribute(string)
		//		Get a attribute by name.
		//-------------------------------------------------
		Attribute GetAttribute(string);*/
		
		vector<Tuple> tuples;
		
		string getTableName();

		void ShowTable();
		
		bool isHidedPK;
		
	protected:
		
		vector<Attribute> attributes;
		
	private:
		
		string tableName;
		
		vector<int> PKIndexes;
};
#endif