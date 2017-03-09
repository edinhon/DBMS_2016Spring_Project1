#include <iostream>
#include "InstructionSet.h"

InstructionSet::InstructionSet()
{
	instructionQueue = new queue<Instruction>;
}

InstructionSet::~InstructionSet()
{
	while(!instructionQueue->empty())
		instructionQueue->pop();
	delete instructionQueue;
}

//--------------
// Instruction popInstruction()
//		Get and delete the upper most instruction.
//--------------
Instruction InstructionSet::popInstruction()
{
	Instruction i = instructionQueue->front();
	instructionQueue->pop();
	return i;
}

//--------------
// void pushInstruction(Instruction)
//		Push a instruction into queue.
//--------------
void InstructionSet::pushInstruction(Instruction c)
{
	instructionQueue->push(c);
}

//--------------
// bool isEmpty()
//		Check the queue is empty or not.
//--------------
bool InstructionSet::isEmpty()
{
	return instructionQueue->empty();
}

