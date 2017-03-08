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
		// Command ParseSingleCommand(Command)
		//		Parse single command into command terms set.
		//---------------------------
		Command ParseSingleCommand(Command c);
		
	private:
		
}
