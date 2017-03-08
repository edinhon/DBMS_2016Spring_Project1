#include <iostream>
#include <Parser.h>
#include <Command.h>
#include <CommandSet.h>

using namespace std;

enum class CommandType {CREATE, INSERT, SELECT};

int main () {

	fstream input_testcase;
	Parser parser = new Parser();
	CommandSet commandSet = new CommandSet();
	
	input_testcase.open ("input testcase.txt", ios::in);
	commandSet = parser.ParseAllCommands(input_testcase);
	
	Command command;
	while(!commandSet.isEmpty()){
		command = parser.ParseSingleCommand(commandSet.popCommand());
		
		switch(command.type){
			case CommandType::CREATE :
				
				break;
			case CommandType::INSERT :
				
				break;
			case CommandType::SELECT :
				
				break;
		}
		//TODO: Deal with the command.
	}
	
	input_testcase.close ();
	return 0;
}


