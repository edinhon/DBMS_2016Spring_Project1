#include <iostream>
#include <CommandSet.h>

CommandSet::CommandSet()
{
	commandQueue = new queue<Command>;
}

CommandSet::~CommandSet()
{
	while(!commandQueue->empty())
		commandQueue->pop();
	delete commandQueue;
}

//--------------
// Command popCommand()
//		Get and delete the upper most command.
//--------------
Command CommandSet::popCommand()
{
	Command c = commandQueue->front();
	commandQueue->pop();
	return c;
}

//--------------
// void pushCommand(Command)
//		Push a command into queue.
//--------------
void CommandSet::pushCommand(Command c)
{
	commandQueue->push(c);
}

//--------------
// bool isEmpty()
//		Check the queue is empty or not.
//--------------
bool CommandSet::isEmpty()
{
	return commandQueue->empty();
}

