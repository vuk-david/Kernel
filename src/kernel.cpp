#include "kernel.h"
#include "ksemQ.h"
#include <iostream.h>

KSemQueue* KernelSem::semaphore_list  = new KSemQueue();
//Queue* KernelSem::blocked_threads = new Queue();

extern void dispatch();

KernelSem::KernelSem(int init)
{
	val = init;
	lock();
		blocked_threads = new Queue();
	unlock();

	semaphore_list->insert(this);
}

KernelSem::~KernelSem()
{
	semaphore_list->remove(this);

	lock();
		delete blocked_threads;
	unlock();
}

extern int lock_cntr;

void KernelSem::block()
{
		lock();
			PCB::running->state = BLOCKED;
			blocked_threads->inject((PCB *)PCB::running);
		unlock();
	unlock();

	dispatch();
}

void KernelSem::unblock()
{
		lock();
			PCB * proc = blocked_threads->deleteHead();

			if (proc == 0)
			{
				unlock();
				return;
			}

			proc->state = ACTIVE;
			Scheduler::put(proc);
		unlock();
	unlock();
}

int KernelSem::wait(Time max_time_to_wait)
{
	if (--val < 0)
	{
		lock();
			if(max_time_to_wait)
				PCB::running->waiting =  1;
			else
				PCB::running->waiting=  0;

			PCB::running->lazy_time = max_time_to_wait;
			block();

		if (PCB::running->lazy_time == 0 && max_time_to_wait)
			return 0;

		PCB::running->lazy_time = 0;
	}


	return 1;
}



int KernelSem::signal(int num_of_signals)
{

	lock();
		int back = 0;

		if(num_of_signals == 0 && val++ < 0)
		{
			unblock();
			return 0;
		}

		if (num_of_signals > 0)
		{
			int hlp = 0;

			if(num_of_signals > -val)
				hlp = -val;
			else
				hlp = num_of_signals;

			for (int i = 0; i < hlp; i++)
			{
				unblock();
			}

			val += num_of_signals;

			if(val)
				back = num_of_signals - val;
			else
				back = num_of_signals;
		}
	unlock();

	return back;
}

void KernelSem::semTick()
{
	semaphore_list->tick();
}

