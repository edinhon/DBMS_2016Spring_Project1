#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <iostream>
#include <queue>
#include "Instruction.h"

using namespace std;

class InstructionSet {
	public:
	
		InstructionSet();
		
		~InstructionSet();
		
		//--------------
		// Instruction popInstruction()
		//		Get and delete the upper most instruction.
		//--------------
		Instruction fetchInstruction();
		void popInstruction ();
		
		//--------------
		// void pushInstruction(Instruction)
		//		Push a instruction into queue.
		//--------------
		void pushInstruction(Instruction);
		
		//--------------
		// bool isEmpty()
		//		Check the queue is empty or not.
		//--------------
		bool isEmpty();
		queue<Instruction> *instructionQueue;
	private:
};
#endif