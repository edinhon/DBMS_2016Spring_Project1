#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>
#include <vector>
#include "Instruction.h"
#include "CreateInst.h"
#include "InsertInst.h"

using namespace std;

class Table {
	public:
		
		//--------------
		// Table(CreateInst)
		//		Create a table and set attributes.
		//--------------
		Table(CreateInst);
		
		~Table();
		
		//--------------
		// void InsertTuple(InsertInst)
		//		Generate a tuple with attribute values.
		//--------------
		void InsertTuple(InsertInst);
		
		class Attribute {
			public:
				string name;
				int type;	// 0 = int, 1 = varchar
				string value;
				bool isPK;
		};
		
		class Tuple {
			public:
				vector<Attribute> values;
				
				//--------------
				// void setValue(string value)
				//		Set value of tuple by index number.
				//		(INSERT without attribute name)
				//--------------
				void setValue(int index, string value);
				
				//--------------
				// void setValue(string name, string value)
				//		Set value of tuple by attribute name.
				//		(INSERT with attribute name)
				//--------------
				void setValue(string name, string value);
				
				//--------------
				// string getValue(string name)
				//		Get value by attribute name.
				//--------------
				string getValue(string name);
		};
		
		vector<Tuple> tuples;
		
	private:
		
		string tableName;
		
		vector<Attribute> attributes;
};
#endif

