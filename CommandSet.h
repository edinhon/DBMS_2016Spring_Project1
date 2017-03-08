#include <iostream>
#include <queue>
#include <Command.h>

using namespace std;

class CommandSet {
	public:
	
		CommandSet();
		
		~CommandSet();
		
		//--------------
		// Command popCommand()
		//		Get and delete the upper most command.
		//--------------
		Command popCommand();
		
		//--------------
		// void pushCommand(Command)
		//		Push a command into queue.
		//--------------
		void pushCommand(Command c);
		
		//--------------
		// bool isEmpty()
		//		Check the queue is empty or not.
		//--------------
		bool isEmpty();
		
	private:
	
		queue<Command> commandQueue*;
}
