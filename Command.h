#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Command {
	public:
		
		Command();
		
		~Command();
		
		void setCommandString(string s);
		
		string getCommandString();
		
		void setTermTokens(vector<string> v*);
		
		vector<string> getTermTokens();
		
	private:
	
		bool isLegal;
		
		string commandString;
		
		vector<string> termTokens*;
		
		int type;
}
