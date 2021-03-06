#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstddef>
#include "Table.h"

//--------------
// Table(CreateInst)
//		Create a table and set attributes.
//--------------
Table::Table(CreateInst *cinst)
{
	tableName = cinst->tableName;
	
	Depot *depot = new Depot(tableName.c_str(), Depot::OWRITER | Depot::OCREAT);
	depot->close();
	
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
		a.from = -1;
		a.isIdx = 0;
		
		attributes.push_back(a);
	}
}

//---------------------------------------------------
// Table()
//		Create a empty table. Used in SELECT instruction.
//---------------------------------------------------
Table::Table()
{
	tableName = "SelectedTable";
	
	isHidedPK = true;
	
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
	if(value == NULL) {
		values[index].value = NULL;
		return;
	}
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
			if(value == NULL) {
				it->value = NULL;
				return;
			}
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

//-----------------
// bool CheckEmpty()
//		Check if this Tuple is a empty Tuple.
//-----------------
bool Table::Tuple::CheckEmpty()
{
	bool flag = true;
	for (int i = 0 ; i < (int)values.size() ; i++){
		if(values[i].value != NULL){
			flag = false;
		}
	}
	return flag;
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
			if(iinst->insertedValueTypes[i] == -1) value = NULL;
			t.setValue(i, value);
		}
	} else {
		for (int i = 0 ; i < (int)iinst->insertedAttributes.size() ; i++){
			string name = iinst->insertedAttributes[i];
			string *value = iinst->insertedValues[i];
			if(iinst->insertedValueTypes[i] == -1) value = NULL;
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

	//Write into disk
	const char* formatted = t.FormatTuple ();
	try{
		Depot* depot = new Depot(tableName.c_str(), Depot::OWRITER | Depot::ONOLCK);
		depot->put(to_string(tuples.size()).c_str(), -1, formatted, -1);
		depot->close();
	} catch(Depot_error& e){
		cerr << e << endl;
	}
	
	for(int i = 0 ; i < (int)attributes.size() ; i++){
		switch(attributes[i].isIdx){
			case 1: {
				//B+ tree
				string idxName = "IDX_BPtree_" + tableName + "_" + attributes[i].name;
				try{
					Villa villa(idxName.c_str(), Villa::OWRITER | Depot::ONOLCK);
					villa.put(t.values[i].value->c_str(), -1, formatted, -1);
					villa.close();
				} catch (Villa_error& e) {
					//cerr << e << endl;
					return;
				}
				break;
			}
			case 2: {
				//Hashing
				string idxName = "IDX_Hash_" + tableName + "_" + attributes[i].name;
				try{
					Depot depot(idxName.c_str(), Depot::OWRITER | Depot::ONOLCK);
					depot.put(t.values[i].value->c_str(), -1, formatted, -1, Depot::DKEEP);
					depot.close();
				} catch (Depot_error& e) {
					//cerr << e << endl;
					return;
				}
				break;
			}
			default: {
				break;
			}
		}
	}
}

//-----------------------
// int InsertEmptyTuple()
//		Generate a empty by SELECT
//-----------------------
int Table::InsertEmptyTuple()
{
	Tuple *t = new Tuple();
	t->values = attributes;
	t->isHidedPK = isHidedPK;
	for (int i = 0 ; i < (int)attributes.size() ; i++){
		t->values[i].value = NULL;
	}
	tuples.push_back(*t);
	
	return ((int)tuples.size() - 1);
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
		if ((int)attributes.size() != (int)iinst->insertedValues.size()){
			cout << "- Error: INSERT value number doesn't match to table attribute number.\n";
			return false;
		}
		
		//Check attribute type of instruction without attribute name.
		for (int i = 0 ; i < (int)attributes.size() ; i++){
			if(iinst->insertedValueTypes[i] != -1 && attributes[i].type != iinst->insertedValueTypes[i]){
				cout << "- Error: INSERT value type doesn't match to table attribute type.\n";
				return false;
			}
		}
		
		//Check NULL value of PK.
		for (int i = 0 ; i < (int)PKIndexes.size() ; i++){
			if (iinst->insertedValueTypes[PKIndexes[i]] == -1){
				cout << "- Error: There exists NULL value in PK.\n";
				return false;
			}
		}
		
		//Check duplicate PK without attribute name.
		string s = "";
		if (isHidedPK){
			for (int i = 0 ; i < (int)iinst->insertedValues.size() ; i++){
				if(iinst->insertedValueTypes[i] != -1)
					s += *(iinst->insertedValues[i]);
			}
		}/*
		if(!isHidedPK){//If there is index of PK in disk, use index to boost speed.
			for(int i = 0 ; i < (int)PKIndexes.size() ; i++){
				string attrName = attributes[PKIndexes[i]].name;
				switch(attributes[PKIndexes[i]].isIdx){
					case 1: {
						//B+tree
						string idxName = "IDX_BPtree_" + tableName + "_" + attrName;
						try{
							Villa* villa = new Villa(idxName.c_str(), Villa::OREADER | Villa::ONOLCK);
							try{
								villa->get(iinst->insertedValues[PKIndexes[i]]->c_str(), -1);
								cout << "- Error: There exists duplicate PK value.\n";
								return false;
							} catch(Villa_error& e){
								//Tuple doesn't exist, and it can be insert.
							}
							villa->close();
						} catch(Villa_error& e){}
						break;
					}
					case 2: {
						//Hash
						string idxName = "IDX_Hash_" + tableName + "_" + attrName;
						try{
							Depot* depot = new Depot(idxName.c_str(), Depot::OREADER | Depot::ONOLCK);
							try{
								depot->get(iinst->insertedValues[PKIndexes[i]]->c_str(), -1);
								cout << "- Error: There exists duplicate PK value.\n";
								return false;
							} catch(Depot_error& e){
								//Tuple doesn't exist, and it can be insert.
							}
							depot->close();
						} catch(Depot_error& e){}
						break;
					}
					default: {
						for(int j = 0 ; j < (int)tuples.size() ; j++){
							if((*(tuples[j].values[PKIndexes[i]].value)).compare(*(iinst->insertedValues[PKIndexes[i]])) == 0){
								cout << "- Error: There exists duplicate PK value.\n";
								return false;
							}
						}
						break;
					}
				} 
			}
		}
		for (int i = 0 ; i < (int)tuples.size() ; i++){
			if (isHidedPK) {
				if (tuples[i].hidedPK != NULL && (*(tuples[i].hidedPK)).compare(s) == 0){
					cout << "- Error: There exists duplicate tuple.\n";
					return false;
				}
			} 
		}*/
		
		for (int i = 0 ; i < (int)tuples.size() ; i++){
			if (isHidedPK) {
				if (tuples[i].hidedPK != NULL && (*(tuples[i].hidedPK)).compare(s) == 0){
					cout << "- Error: There exists duplicate tuple.\n";
					return false;
				}
			} else{
				for(int j = 0 ; j < (int)PKIndexes.size() ; j++){
					if((*(tuples[i].values[PKIndexes[j]].value)).compare(*(iinst->insertedValues[PKIndexes[j]])) == 0){
						cout << "- Error: There exists duplicate PK value.\n";
						return false;
					}
				}
			}
		}
		
		
		//Check varchar size.
		for (int i = 0 ; i < (int)attributes.size() ; i++){
			if(attributes[i].type == 1 && 
				iinst->insertedValueTypes[i] != -1 && 
				((int)(*(iinst->insertedValues[i])).size() > attributes[i].varCharSize)){
					cout << "- Error: Different varchar size with table.\n";
					return false;
				}
		}
		
	} else {
		
		if(iinst->insertedAttributes.size() != iinst->insertedValues.size()){
			cout << "- Error: INSERT attribute name number doesn't match to value number\n";
			return false;
		}
		
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
					if (iinst->insertedValueTypes[i] != -1 && 
						attributes[j].type != iinst->insertedValueTypes[i]){
							cout << "- Error: INSERT value type doesn't match to table attribute type.\n";
							return false;
					}
					//For varchar size
					if(attributes[j].type == 1 && 
						((int)((*(iinst->insertedValues[i])).size()) > attributes[j].varCharSize)){
							cout << "- Error: Different varchar size with table.\n";
							return false;
						}
				}
			}
			if(nameChecker == false){
				cout << "- Error: INSERT name doesn't match to table attribute name.\n";
				return false;
			}
		}
		
		//Check NULL value and duplicate value of PK.
		//Check duplicate PK without attribute name.
		if (!isHidedPK){
			bool nullPKChecker;
			for (int i = 0 ; i < (int)PKIndexes.size() ; i++){
				nullPKChecker = false;
				string n1 = attributes[PKIndexes[i]].name;
				transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
				for (int j = 0 ; j < (int)iinst->insertedAttributes.size() ; j++){
					string n2 = iinst->insertedAttributes[j];
					transform(n2.begin(), n2.end(), n2.begin(), ::tolower);
					if(n1.compare(n2) == 0){
						//For NULL
						if(iinst->insertedValueTypes[PKIndexes[i]] != -1)
							nullPKChecker = true;
						//For duplicate
						for (int k = 0 ; k < (int)tuples.size() ; k++){
							if((*(tuples[k].values[PKIndexes[i]].value)).compare(*(iinst->insertedValues[j])) == 0){
								cout << "- Error: There exists duplicate PK value.\n";
								return false;
							}
						}
					}
				}
				if(nullPKChecker == false){
					cout << "- Error: There exists NULL value in PK.\n";
					return false;
				}
			}
		} else {
			string s = "";
			for (int j = 0 ; j < (int)attributes.size() ; j++){
				string n1 = attributes[j].name;
				transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
				for (int i = 0 ; i < (int)iinst->insertedAttributes.size() ; i++){
					string n2 = iinst->insertedAttributes[i];
					transform(n2.begin(), n2.end(), n2.begin(), ::tolower);
					if(n1.compare(n2) == 0 && iinst->insertedValueTypes[i] != -1)
						s += *(iinst->insertedValues[i]);
				}
			}
			
			
			for (int i = 0 ; i < (int)tuples.size() ; i++){
				if ((*(tuples[i].hidedPK)).compare(s) == 0){
					cout << "- Error: There exists duplicate tuple.\n";
					return false;
				}
			}
		}
		
	}
	
	return true;
}

//-------------------------------------------------
// bool CopyAttribute(Table*, string, int)
//		Copy a new attribute column from input Table into a this empty Table.
//-------------------------------------------------
bool Table::CopyAttribute(Table* t, string attrName, int TIndex)
{
	if(tuples.size() != 0){
		cout << "- Error: Cannot copy the attribute into a non-empty table.\n";
		return false;
	}
	
	string n1 = attrName;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	if(ContainAttribute(attrName)){
		int AIndex = -1;
		for(int i = 0 ; i < (int)attributes.size() ; i++){
			string n2 = attributes[i].name;
			transform(n2.begin(), n2.end(), n2.begin(),::tolower);
			
			if(n1.compare(n2) == 0){
				AIndex = i;
				break;
			}
		}
		
		if(AIndex == TIndex || AIndex == -1){
			cout << "- Error: There exists the same attribute " << attrName <<
			" in this table.\n";
			return false;
		}
	}
	
	for(int i = 0 ; i < (int)t->attributes.size() ; i++){
		string n2 = t->attributes[i].name;
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0){
			Attribute a = t->attributes[i];
			a.isPK = false;
			a.from = TIndex;
			attributes.push_back(a);
			return true;
		}
	}
	return false;
}

//-------------------------------------------------
// bool CopyAttributes(Table*, int)
//		Copy all attribute columns of input Table into this empty Table.
//-------------------------------------------------
bool Table::CopyAttributes(Table* t, int TIndex)
{
	for(int i = 0 ; i < (int)t->attributes.size() ; i++){
		if(!CopyAttribute(t, t->attributes[i].name, TIndex))
			return false;
	}
	return true;
}

//-------------------------------------------------
// bool ContainAttribute(string)
//		Check the attribute by name in table or not.
//-------------------------------------------------
bool Table::ContainAttribute(string name)
{
	string n1 = name;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for(int i = 0 ; i < (int)attributes.size() ; i++){
		string n2 = attributes[i].name;
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0)
			return true;
	}
	return false;
}

//-------------------------------------------------
// int GetAttributeType(string)
//		Get a attribute type by name.
//-------------------------------------------------
int Table::GetAttributeType(string name)
{
	string n1 = name;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for(int i = 0 ; i < (int)attributes.size() ; i++){
		string n2 = attributes[i].name;
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0)
			return attributes[i].type;
	}
	
	return -1;
}

//-------------------------------------------------
// bool CopyValueToTuple(Table*, string, int, int)
//		Copy a tuple value into the index tuple of Table.
//-------------------------------------------------
bool Table::CopyValueToTuple(Table* t, string attrName, int in, int out)
{
	int inAttrIndex = -1;
	int outAttrIndex = -1;
	
	string n1 = attrName;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for(int i = 0 ; i < (int)attributes.size() ; i++){
		string n2 = attributes[i].name;
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0){
			inAttrIndex = i;
			break;
		}
	}
	if(inAttrIndex == -1){
		cout << "- Error: Cannot find " << attrName << " in table " << 
			getTableName() << endl;
		return false;
	}
	
	for(int i = 0 ; i < (int)t->attributes.size() ; i++){
		string n2 = t->attributes[i].name;
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0){
			outAttrIndex = i;
			break;
		}
	}
	if(outAttrIndex == -1){
		cout << "- Error: Cannot find " << attrName << " in table " << 
			t->getTableName() << endl;
		return false;
	}
	
	tuples[in].values[inAttrIndex].value = new string(*(t->tuples[out].values[outAttrIndex].value));
	return true;
}

//-------------------------------------------------
// bool CopyValuesToTuple(Table*, int, int)
//		Copy all tuples value into the index tuple of Table.
//-------------------------------------------------
bool Table::CopyValuesToTuple(Table* t, int in, int out)
{
	for(int i = 0 ; i < (int)t->attributes.size() ; i++){
		string n1 = t->attributes[i].name;
		transform(n1.begin(), n1.end(), n1.begin(),::tolower);
		
		for(int j = 0 ; j < (int)attributes.size() ; j++){
			string n2 = attributes[j].name;
			transform(n2.begin(), n2.end(), n2.begin(),::tolower);
			
			if(n1.compare(n2) == 0){
				tuples[in].values[j].value = new string(*(t->tuples[out].values[i].value));
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
	
	//Set attribute print sizes
	for (int i = 0 ; i < (int)attributes.size() ; i++){
		//Type = int, 11 size char, 2 space
		if(attributes[i].type == 0){
			int maxLength;
			if((int)attributes[i].name.size() >= 11){
				maxLength = (int)attributes[i].name.size();
			} else maxLength = 11;
			printSize.push_back(maxLength);
		} 
		//Type = varchar, varCharSize size char, 2space
		else if (attributes[i].type == 1){
			int maxLength;
			if((int)attributes[i].name.size() >= attributes[i].varCharSize){
				maxLength = (int)attributes[i].name.size();
			} else maxLength = attributes[i].varCharSize;
			printSize.push_back(maxLength);
		}
	}
	
	//Line in above
	cout << "  ";
	for (int i = 0 ; i < (int)printSize.size() ; i++){
		for (int j = 0 ; j < printSize[i] ; j++){
			cout << "-";
		}
		if(i != ((int)printSize.size()-1)) cout << "-";
		else cout << " ";
	}
	cout << endl;
	
	//Print attribute names
	cout << " |";
	for (int i = 0 ; i < (int)attributes.size() ; i++){
		for(int j = 0 ; j < (printSize[i] - (int)attributes[i].name.size()) ; j++){
			cout << " ";
		}
		cout << attributes[i].name << "|";
	}
	cout << endl;
	
	//Line in bottom
	cout << "  ";
	for (int i = 0 ; i < (int)printSize.size() ; i++){
		for (int j = 0 ; j < printSize[i] ; j++){
			cout << "-";
		}
		if(i != ((int)printSize.size()-1)) cout << "-";
		else cout << " ";
	}
	cout << endl;
		
	//Print out tuples
	for (int i = 0 ; i < (int)tuples.size() ; i++){
		cout << " |" ;
		for (int j = 0 ; j < (int)tuples[i].values.size() ; j++){
			if(tuples[i].values[j].value != NULL){
				for (int k = 0 ; k < (printSize[j] - (int)((*(tuples[i].values[j].value)).size())) ; k++){
					cout << " ";
				}
				cout << *(tuples[i].values[j].value) << "|";
			} else {
				for (int k = 0 ; k < printSize[j] ; k++){
					cout << " ";
				}
				cout << "|";
			}
		}
		cout << endl;
		
		//Line in bottom
		cout << "  ";
		for (int k = 0 ; k < (int)printSize.size() ; k++){
			for (int j = 0 ; j < printSize[k] ; j++){
				cout << "-";
			}
			if(k != ((int)printSize.size()-1)) cout << "-";
			else cout << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Table::ShowTable(SelectInst* sinst)
{
	vector<int> printSize;
	
	//Print out table name
	cout << tableName << endl;
	
	//Set attribute print sizes
	for (int i = 0 ; i < (int)attributes.size() ; i++){
		//Type = int, 11 size char, 2 space
		if(attributes[i].type == 0){
			int maxLength;
			if(((int)attributes[i].name.size() + (int)sinst->tableNames[attributes[i].from].size() + 1) >= 11){
				maxLength = ((int)attributes[i].name.size() + (int)sinst->tableNames[attributes[i].from].size() + 1);
			} else maxLength = 11;
			printSize.push_back(maxLength);
		} 
		//Type = varchar, varCharSize size char, 2space
		else if (attributes[i].type == 1){
			int maxLength;
			if(((int)attributes[i].name.size() + (int)sinst->tableNames[attributes[i].from].size() + 1) >= attributes[i].varCharSize){
				maxLength = ((int)attributes[i].name.size() + (int)sinst->tableNames[attributes[i].from].size() + 1);
			} else maxLength = attributes[i].varCharSize;
			printSize.push_back(maxLength);
		}
	}
	
	//Line in above
	cout << "  ";
	for (int i = 0 ; i < (int)printSize.size() ; i++){
		for (int j = 0 ; j < printSize[i] ; j++){
			cout << "-";
		}
		if(i != ((int)printSize.size()-1)) cout << "-";
		else cout << " ";
	}
	cout << endl;
	
	//Print attribute names
	cout << " |";
	for (int i = 0 ; i < (int)attributes.size() ; i++){
		for(int j = 0 ; j < (printSize[i] - (int)attributes[i].name.size() - (int)sinst->tableNames[attributes[i].from].size() - 1) ; j++){
			cout << " ";
		}
		cout << sinst->tableNames[attributes[i].from] << "." << attributes[i].name << "|";
	}
	cout << endl;
	
	//Line in bottom
	cout << "  ";
	for (int i = 0 ; i < (int)printSize.size() ; i++){
		for (int j = 0 ; j < printSize[i] ; j++){
			cout << "-";
		}
		if(i != ((int)printSize.size()-1)) cout << "-";
		else cout << " ";
	}
	cout << endl;
		
	//Print out tuples
	for (int i = 0 ; i < (int)tuples.size() ; i++){
		cout << " |" ;
		for (int j = 0 ; j < (int)tuples[i].values.size() ; j++){
			if(tuples[i].values[j].value != NULL){
				for (int k = 0 ; k < (printSize[j] - (int)((*(tuples[i].values[j].value)).size())) ; k++){
					cout << " ";
				}
				cout << *(tuples[i].values[j].value) << "|";
			} else {
				for (int k = 0 ; k < printSize[j] ; k++){
					cout << " ";
				}
				cout << "|";
			}
		}
		cout << endl;
		
		//Line in bottom
		cout << "  ";
		for (int k = 0 ; k < (int)printSize.size() ; k++){
			for (int j = 0 ; j < printSize[k] ; j++){
				cout << "-";
			}
			if(k != ((int)printSize.size()-1)) cout << "-";
			else cout << " ";
		}
		cout << endl;
	}
	cout << endl;
}

//---------------------------
// void Count_ShowTable(SelectInst*)
//---------------------------
void Table::Count_ShowTable(SelectInst* sinst)
{
	int printSize = 0;
	
	string attrName = "COUNT(";
	for(int i = 0 ; i < (int)sinst->selectedAttributesNames.size() ; i++){
		if(i != 0)
			attrName += ", ";
		if(sinst->selectedAttributesTables[i].compare("") != 0)
			attrName += (sinst->selectedAttributesTables[i] + ".");
		attrName += sinst->selectedAttributesNames[i];
	}
	attrName += ")";
	
	stringstream ss;
	ss << tuples.size();
	
	if(ss.str().size() >= attrName.size()){
		printSize = ss.str().size();
	} else printSize = attrName.size();
	
	//Line in above
	cout << "  ";
	for (int j = 0 ; j < printSize ; j++){
		cout << "-";
	}
	cout << " " << endl;
	
	//Print attribute names
	cout << " |";
	for(int i = 0 ; i < (int)(printSize - attrName.size()) ; i++){
		cout << " ";
	}
	cout << attrName << "|" << endl;
	
	//Line in bottom
	cout << "  ";
	for (int j = 0 ; j < printSize ; j++){
		cout << "-";
	}
	cout << " " << endl;
		
	//Print out COUNT
	cout << " |" ;
	for(int i = 0 ; i < (int)(printSize - ss.str().size()) ; i++){
		cout << " ";
	}
	cout << tuples.size() << "|" << endl;
	
	//Line in bottom
	cout << "  ";
	for (int j = 0 ; j < printSize ; j++){
		cout << "-";
	}
	cout << " " << endl;
}

//---------------------------
// void Sum_ShowTable(SelectInst*)
//---------------------------
void Table::Sum_ShowTable(SelectInst* sinst)
{
	
	int sumArr[(int)attributes.size()];
	int sumLenArr[(int)attributes.size()];
	string sumAttr[(int)attributes.size()];
	vector<int> printSize;
	
	//initial attribute string
	for(int i = 0 ; i < (int)attributes.size() ; i++){
		sumAttr[i] = "SUM(";
	}
	
	//Set sum value and attribute print sizes
	for(int i = 0 ; i < (int)attributes.size() ; i++){
		if(attributes[i].type == 0){
			int sum = 0;
			for(int j = 0 ; j < (int)tuples.size() ; j++){
				sum += atoi((*(tuples[j].values[i].value)).c_str());
			}
			sumArr[i] = sum;
			
			stringstream ss;
			ss << sumArr[i];
			sumLenArr[i] = ss.str().size();
			
			if(sinst->selectedAttributesTables[i].compare("") != 0)
				sumAttr[i] += (sinst->selectedAttributesTables[i] + ".");
			sumAttr[i] += attributes[i].name + ")";
			
			int maxLength;
			if(sumAttr[i].size() >= 11){
				maxLength = sumAttr[i].size();
			} else maxLength = 11;
			printSize.push_back(maxLength);
		}
	}
	
	//Line in above
	cout << "  ";
	for (int i = 0 ; i < (int)printSize.size() ; i++){
		for (int j = 0 ; j < printSize[i] ; j++){
			cout << "-";
		}
		if(i != ((int)printSize.size()-1)) cout << "-";
		else cout << " ";
	}
	cout << endl;
	
	//Print attribute names
	cout << " |";
	for (int i = 0 ; i < (int)attributes.size() ; i++){
		for(int j = 0 ; j < (printSize[i] - (int)sumAttr[i].size()) ; j++){
			cout << " ";
		}
		cout << sumAttr[i] << "|";
	}
	cout << endl;
	
	//Line in bottom
	cout << "  ";
	for (int i = 0 ; i < (int)printSize.size() ; i++){
		for (int j = 0 ; j < printSize[i] ; j++){
			cout << "-";
		}
		if(i != ((int)printSize.size()-1)) cout << "-";
		else cout << " ";
	}
	cout << endl;
	
	//Print out tuples
	cout << " |" ;
	for (int i = 0 ; i < (int)attributes.size() ; i++){
		
		for (int j = 0 ; j < (printSize[i] - (int)sumLenArr[i]) ; j++){
			cout << " ";
		}
		cout << sumArr[i] << "|";
	}
	cout << endl;
		
	//Line in bottom
	cout << "  ";
	for (int i = 0 ; i < (int)printSize.size() ; i++){
		for (int j = 0 ; j < printSize[i] ; j++){
			cout << "-";
		}
		if(i != ((int)printSize.size()-1)) cout << "-";
		else cout << " ";
	}
	cout << endl;
}

//-----------------------------------------------
// bool CreateIndex(CreateIndexInst* ciinst)
//		Create index structure.
//-----------------------------------------------
bool Table::CreateIndex(CreateIndexInst* ciinst){
	switch(ciinst->IdxType){
		case 1:{
			for(int k = 0 ; k < (int)ciinst->attrName.size() ; k++){
				string idxName = "IDX_BPtree_" + tableName + "_" + ciinst->attrName[k];
				
				if(ContainAttribute(ciinst->attrName[k])){
					
					int attrIdx = SetAttributeIndex(ciinst->attrName[k], ciinst->IdxType);
					if(attrIdx == -1) 
						return false;
					
					try{
						Villa villa(idxName.c_str(), Villa::OWRITER | Villa::OCREAT | Villa::OTRUNC);
						for(int i = 0 ; i < (int)tuples.size() ; i++){
							const char* formatted = tuples[i].FormatTuple ();
							villa.put(tuples[i].values[attrIdx].value->c_str(), -1, formatted, -1);
						}
						villa.close();
					} catch (Villa_error& e){
						cerr << e << endl;
					}
				}
			}
			
			return true;
			break;
		}
		case 2:{
			for(int k = 0 ; k < (int)ciinst->attrName.size() ; k++){
				string idxName = "IDX_Hash_" + tableName + "_" + ciinst->attrName[k];
				
				if(ContainAttribute(ciinst->attrName[k])){
					
					int attrIdx = SetAttributeIndex(ciinst->attrName[k], ciinst->IdxType);
					if(attrIdx == -1) 
						return false;
					
					try{
						Depot depot(idxName.c_str(), Depot::OWRITER | Depot::OCREAT | Depot::OTRUNC);
						for(int i = 0 ; i < (int)tuples.size() ; i++){
							const char* formatted = tuples[i].FormatTuple ();
							depot.put(tuples[i].values[attrIdx].value->c_str(), -1, formatted, -1, Depot::DKEEP);
						}
						depot.close();
					} catch(Depot_error& e){
						cerr << e << endl;
					}
				}
			}
			return true;
			break;
		}
		default:{
			cout << "- Error: Index type error.\n";
			return false;
			break;
		}
	}
}

//-----------------------------------------------
// int SetAttributeIndex(string attrName, int mode)
//		Set attribute isIdx and write into data, 
//	parameter mode implies which structure is used, 
//	1 = B+ tree, 2 = Hashing, and return attribute index.
//-----------------------------------------------
int Table::SetAttributeIndex(string attrName, int mode){
	
	string n1 = attrName;
	transform(n1.begin(), n1.end(), n1.begin(),::tolower);
	
	for(int i = 0 ; i < (int)attributes.size() ; i++){
		string n2 = attributes[i].name;
		transform(n2.begin(), n2.end(), n2.begin(),::tolower);
		
		if(n1.compare(n2) == 0){
			attributes[i].isIdx = mode;
			InformationWrite_Table();
			return i;
		}
	}
	return -1;
}

//-----------------------------------------------
// a function to store table information in disk
//-----------------------------------------------
void Table::InformationWrite_Table ()
{
	// TableInformationFile
	fstream* tableInformation= new fstream();
	tableInformation->open (TableInformationFile, ios::out);
	
	*tableInformation << tableName << endl;
	for (int i=0; i<(int)attributes.size(); i++) {
		*tableInformation << attributes[i].name << endl << attributes[i].type << endl
			<< attributes[i].varCharSize << endl << attributes[i].isPK << endl << attributes[i].isIdx << endl;
	}
	
}
void Table::InformationRead_Table (string fileName)
{
	Attribute* a;
	string inputString;
	int step = 0;
	int numOfAttributes = 0;
	// TableInformationFile
	fstream tableInformation;
	tableInformation.open (fileName, ios::in);
	if (!tableInformation) {
		cout << "- No Existing Table\n- Initializing..." << endl;
		return;
	}
	getline (tableInformation, inputString);
	tableName = inputString;

	while (getline (tableInformation, inputString)) {
		// dealing with each tuples
		switch (step) {
			case 0 : {
				a = new Attribute ();
				a->name = inputString;
				a->from = -1;
				step += 1;
				break;
			}
			case 1 : {
				a->type = atoi(inputString.c_str());
				step += 1;
				break;
			}
			case 2 : {
				a->varCharSize = atoi(inputString.c_str());
				step += 1;
				break;
			}
			case 3 : {
				if (inputString == "1")
					a->isPK = true;
				else
					a->isPK = false;

				if (a->isPK){
					PKIndexes.push_back(numOfAttributes);
					isHidedPK = false;
				}
				step += 1;
				break;
			}
			case 4 : {
				a->isIdx = atoi(inputString.c_str());
				attributes.push_back(*a);
				numOfAttributes += 1;
				step = 0;
				break;
			}
		}
	}
	tableInformation.close();
}

//-----------------------------------------------
// void LoadTuple()
//		Load database data from dbfile.
//-----------------------------------------------
void Table::LoadTable () 
{
	char* key;
	char* loadedVal;
	Depot* depot = new Depot(tableName.c_str(), Depot::OREADER);

	depot->iterinit();

	key = depot->iternext();
	while(1){
		try {
			loadedVal = depot->get(key, -1);
			
			Tuple *t = new Tuple ();
			t->isHidedPK = isHidedPK;
			t->values = attributes;
			t->LoadTuple (loadedVal);
			tuples.push_back (*t);
			
			key = depot->iternext ();
		} catch (Depot_error& e) {
			cerr << e << endl;
			return;
		}
	}
	
	depot->close();
}


// format the tuple to store into disk
const char* Table::Tuple::FormatTuple () 
{
	string* output = new string();
	int numOfAttributes = values.size();
	
	for (int i=0; i<numOfAttributes; i++) {
		int type = values[i].type;
		switch (type) {
			case 0 : {	// int, size = 11
				string thisValue;

				if(values[i].value != NULL){
					for (int k = 0 ; k < (11 - (int)((*(values[i].value)).size())) ; k++){
						thisValue += "\4";
					}
					thisValue += *(values[i].value);
					thisValue += "1";
				} else {
					for (int k = 0 ; k < 11 ; k++){
						thisValue += "\4";
					}
					thisValue += "0";
				}
				
				*output += thisValue;
				break;
			}
			case 1 : {	// varchar
				string thisValue;
				
				if(values[i].value != NULL){
					for (int k = 0 ; k < (values[i].varCharSize - (int)((*(values[i].value)).size())) ; k++){
						thisValue += "\4";
					}
					thisValue += *(values[i].value);
					thisValue += "1";
				} else {
					for (int k = 0 ; k < values[i].varCharSize ; k++){
						thisValue += "\4";
					}
					thisValue += "0";
				}
				
				*output += thisValue;
				break;
			}
		}
	}
	
	return (*output).c_str();
}

void Table::Tuple::LoadTuple (char* input)
{
	int readPtr = 0;
	string inputStr(input);
	
	for(int i = 0 ; i < (int)values.size() ; i++){
		switch(values[i].type){
			case 0: {
				//Integer
				string tmp = inputStr.substr(readPtr, 11);
				readPtr += 11;
				string nullByte = inputStr.substr(readPtr, 1);
				readPtr += 1;
				
				if(nullByte == "0"){
					values[i].value = NULL;
				} else if (nullByte == "1"){
					size_t pos = tmp.find_first_not_of("\4");
					if(pos == string::npos){
						values[i].value = new string("");
					} else {
						values[i].value = new string(tmp.substr(pos));
					}
				}
				
				break;
			}
			case 1: {
				//String
				string tmp = inputStr.substr(readPtr, values[i].varCharSize);
				readPtr += values[i].varCharSize;
				string nullByte = inputStr.substr(readPtr, 1);
				readPtr += 1;
				
				if(nullByte == "0"){
					values[i].value = NULL;
				} else if (nullByte == "1"){
					size_t pos = tmp.find_first_not_of("\4");
					if(pos == string::npos){
						values[i].value = new string("");
					} else {
						values[i].value = new string(tmp.substr(pos));
					}
				}
				
				break;
			}
		}
	}
	
	if (isHidedPK){
		string s = "";
		for (int i = 0 ; i < (int)values.size() ; i++){
			if(values[i].value != NULL) {
				s += *(values[i].value);
			}
		}
		hidedPK = new string(s);
	}
}
