#include <iostream>
#include <algorithm>
#include <sstream>
#include "Table.h"

//--------------
// Table(CreateInst)
//		Create a table and set attributes.
//--------------
Table::Table(CreateInst *cinst)
{
	tableName = cinst->tableName;
	
	depot = new Depot(tableName.c_str(), Depot::OWRITER | Depot::OCREAT);
	
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
			if (iinst->insertedValueTypes[i] == -1){
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
		}
		for (int i = 0 ; i < (int)tuples.size() ; i++){
			if (isHidedPK) {
				if (tuples[i].hidedPK != NULL && (*(tuples[i].hidedPK)).compare(s) == 0){
					cout << "- Error: There exists duplicate tuple.\n";
					return false;
				}
			} else {
				for(int j = 0 ; j < (int)PKIndexes.size() ; j++){
					if((*(tuples[i].values[j].value)).compare(*(iinst->insertedValues[j])) == 0){
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
				string n1 = attributes[i].name;
				transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
				for (int j = 0 ; j < (int)iinst->insertedAttributes.size() ; j++){
					string n2 = iinst->insertedAttributes[i];
					transform(n2.begin(), n2.end(), n2.begin(), ::tolower);
					if(n1.compare(n2) == 0){
						//For NULL
						if(iinst->insertedValueTypes[i] != -1)
							nullPKChecker = true;
						//For duplicate
						for (int k = 0 ; k < (int)tuples.size() ; k++){
							if((*(tuples[k].values[i].value)).compare(*(iinst->insertedValues[j])) == 0){
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

