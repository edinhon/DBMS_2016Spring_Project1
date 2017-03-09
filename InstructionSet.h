#include <iostream>
#include <queue>
#include <Instruction.h>

using namespace std;

class InstructionSet {
	public:
	
		InstructionSet();
		
		~InstructionSet();
		
		//--------------
		// Instruction popInstruction()
		//		Get and delete the upper most instruction.
		//--------------
		Instruction popInstruction();
		
		//--------------
		// void pushInstruction(Instruction)
		//		Push a instruction into queue.
		//--------------
		void pushInstruction(Instruction c);
		
		//--------------
		// bool isEmpty()
		//		Check the queue is empty or not.
		//--------------
		bool isEmpty();
		
	private:
	
		queue<Instruction> instructionQueue*;
}
