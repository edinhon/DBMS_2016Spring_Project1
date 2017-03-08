#include <iostream>
#include <string>
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
		// StringTokens ParseSingleCommand(Command)
		//		Parse single command into command terms set.
		//---------------------------
		void ParseSingleCommand(Command c);
		
	private:
		
}
