#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Command.h>
#include <CommandSet.h>

using namespace std;

class Parser {
	public:
		
		Parser();
		
		//---------------------------
		// CommandSet ParseAllCommands(fstream)
		//		Parse all input commands into single commands set.
		//---------------------------
		CommandSet ParseAllCommands(fstream inputFile);
		
		//---------------------------
		// vector<string> ParseSingleCommand(Command)
		//		Parse single command into command terms set.
		//---------------------------
		vector<string> ParseSingleCommand(Command c);
		
	private:
		
}
