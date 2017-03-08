#include <iostream>
#include <string>
#include <Command.h>

Command::Command()
{
	isLegal = false;
}

Command::~Command()
{
	termTokens->clear();
	delete termTokens;
}

void Command::setCommandString(string s)
{
	commandString = s;
}

string Command::getCommandString()
{
	return commandString;
}

void Command::setTermTokens(vector<string> v*)
{
	termTokens = v;
}

vector<string> Command::getTermTokens()
{
	return termTokens;
}