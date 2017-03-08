#include <iostream>
#include <Parser.h>
#include <Command.h>
#include <CommandSet.h>

using namespace std;



int main () {

	fstream input_testcase;
	Parser parser = new Parser();
	CommandSet commandSet = new CommandSet();
	
	input_testcase.open ("input testcase.txt", ios::in);
	
	commandSet = parser.ParseAllCommands(input_testcase);
	while(!commandSet.isEmpty()){
		parser.ParseSingleCommand(commandSet.popCommand());
		
		//TODO: Deal with the command.
	}
	
	input_testcase.close ();
	return 0;
}


