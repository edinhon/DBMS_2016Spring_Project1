#ifndef TABLE_H
#define TABLE_H

#define TableInformationFile tableName + "InformationFile.idx"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Instruction.h"
#include "CreateInst.h"
#include "InsertInst.h"
#include "SelectInst.h"
#include <xadbm.h>
#include <xdepot.h>
#include <xvilla.h>

using namespace std;
using namespace qdbm;

class Table {
	public:
		
		class Attribute {
			public:
				string name;
				int type;	// 0 = int, 1 = varchar
				int varCharSize; // size of varchar
				string *value;
				bool isPK;
				int from;	//Only use in SELECT
				int isIdx;	//Only use in INDEX, 0 = None, 1 = BP, 2 = Hash
				Attribute& operator= (Attribute const &that){
					if (this != &that) {
						name = that.name;
						type = that.type;
						varCharSize = that.varCharSize;
						value = that.value;
						isPK = that.isPK;
						from = that.from;
						isIdx = that.isIdx;
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
				
				//-----------------
				// bool CheckEmpty()
				//		Check if this Tuple is a empty Tuple.
				//-----------------
				bool CheckEmpty();
				
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
		
		
		//-----------------------
		// int InsertEmptyTuple()
		//		Generate a empty by SELECT
		//-----------------------
		int InsertEmptyTuple();
		
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
		// bool CopyAttribute(Table*, string, int)
		//		Copy a new attribute column into a empty Table.
		//-------------------------------------------------
		bool CopyAttribute(Table*, string, int);
		
		//-------------------------------------------------
		// bool CopyAttributes(Table*, int)
		//		Copy all attribute columns of input Table into a empty Table.
		//-------------------------------------------------
		bool CopyAttributes(Table*, int);
		
		//-------------------------------------------------
		// bool ContainAttribute(string)
		//		Check the attribute by name in table or not.
		//-------------------------------------------------
		bool ContainAttribute(string);
		
		//-------------------------------------------------
		// int GetAttributeType(string)
		//		Get a attribute type by name.
		//-------------------------------------------------
		int GetAttributeType(string);
		
		//-------------------------------------------------
		// bool CopyValueToTuple(Table*, string, int, int)
		//		Copy a tuple value into the index tuple of Table.
		//-------------------------------------------------
		bool CopyValueToTuple(Table*, string, int, int);
		
		//-------------------------------------------------
		// bool CopyValuesToTuple(Table*, int, int)
		//		Copy all tuples value into the index tuple of Table.
		//-------------------------------------------------
		bool CopyValuesToTuple(Table*, int, int);
		
		vector<Tuple> tuples;
		
		string getTableName();

		void ShowTable();
		
		void ShowTable(SelectInst*);
		
		void Count_ShowTable(SelectInst*);
		
		void Sum_ShowTable(SelectInst*);
		
		//-----------------------------------------------
		// bool CreateIndex(string attrName, int mode)
		//		Create index structure, parameter mode implies
		//	which structure is used, 1 = B+ tree, 2 = Hashing.
		//-----------------------------------------------
		bool CreateIndex(string, int);
		
		//-----------------------------------------------
		// bool SetAttributeIndex(string attrName, int mode)
		//		Set attribute isIdx and write into data, 
		//	parameter mode implies which structure is used, 
		//	1 = B+ tree, 2 = Hashing.
		//-----------------------------------------------
		bool SetAttributeIndex(string, int);

		//-----------------------------------------------
		// a function to store table information in disk
		//-----------------------------------------------
		void InformationWrite_Table ();
		void InformationRead_Table (string);

		string returnTableFileName () {
			string tmpt(TableInformationFile);
			return tmpt;
		}
		
		bool isHidedPK;
		
		friend class TableSet;
		
	protected:
		
		vector<Attribute> attributes;
		
	private:
		
		string tableName;
		
		vector<int> PKIndexes;
		
		Depot *mainData;
		
		vector<Depot*> depots;
		
		vector<Villa*> villas;
};
#endif
