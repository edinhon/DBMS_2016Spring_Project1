#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Instruction {
	public:
		
		Instruction();
		
		~Instruction();
		
		//----------------
		// void setInstructionString(string)
		//		Set complete single instruction string.
		//----------------
		void setInstructionString(string s);
		
		//----------------
		// string getInstructionString()
		//		Get complete single instruction string (to parse into tokens).
		//----------------
		string getInstructionString();
		
		//----------------
		// void setTermTokens(vector<string>);
		//		Set tokens from instruction string.
		//----------------
		void setTermTokens(vector<string> v*);
		
		//----------------
		// vector<string> getTermTokens();
		//		Get tokens to run this instruction.
		//----------------
		vector<string> getTermTokens();
		
		bool isLegal;
		
		int type;
		
	private:
		
		string instructionString;
		
		vector<string> termTokens*;
		
}
