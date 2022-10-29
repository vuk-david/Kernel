#include <iostream.h>
#include "thread.h"
#include "queue.h"
#include "kernel.h"
#include "SCHEDULE.h"

Thread::Thread (StackSize stack_size, Time time_slice)
{
	lock();
		myPCB = new PCB(stack_size, time_slice, this);
	unlock();
}

Thread::~Thread()
{
	lock();
		delete myPCB;
	unlock();
}

ID Thread::getId()
{
	return myPCB->my_ID;
}

ID Thread::getRunningId()
{
	return PCB::running->my_ID;
}


Thread* Thread::getThreadById(ID id)
{

	if (id == 1)
	{
		return PCB::idle;
	}

	PCB* process = PCB::all_threads->find(id);

	if (process)
		return process->my_thread;

	return 0;
}

void Thread::start()
{
	myPCB->start();
}

void Thread::waitToComplete()
{


	if(myPCB != PCB::running && myPCB->state != FINISHED)
	{
		myPCB->my_sem->wait(0);

	}

}
