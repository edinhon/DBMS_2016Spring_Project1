#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Instruction {
	public:
		
		Instruction();
		
		~Instruction();
		
		void setInstructionString(string s);
		
		string getInstructionString();
		
		void setTermTokens(vector<string> v*);
		
		vector<string> getTermTokens();
		
	private:
	
		bool isLegal;
		
		string instructionString;
		
		vector<string> termTokens*;
		
		int type;
}
