#include <iostream>
#include <string>
#include <vector>

using namespace std;

class CreateInst : public Instruction {
	public:
		
		CreateInst();
		
		string tableName;
		
		string attributesNames[10];
		
		string attributesTypes[10];
		
		bool isPK[10];
		
	private:
		
		
}
