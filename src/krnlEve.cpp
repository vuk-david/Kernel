#include "krnlEve.h"
#include "pcb.h"

KernelEv::KernelEv(IVTNo iVTNo): blocked(0)
{
	creator_id = PCB::running->my_ID;
	IVTEntry::getIVTEntry(iVTNo)->setKernelEv(this);
}

KernelEv::~KernelEv()
{
	lock();
		delete blocked;
	unlock();
}



void KernelEv::wait()
{
	if (PCB::running->my_ID != creator_id)
		return; // Error

	lock();
		block();
}


void KernelEv::signal()
{
	lock();
		unblock();
}

void KernelEv::block()
{
		PCB::running->state = BLOCKED;
		blocked = (PCB *) PCB::running;
	unlock();
	dispatch();
}

void KernelEv::unblock()
{
		if (blocked == 0)
		{
			unlock();
			return;
		}

		blocked->state = ACTIVE;
		Scheduler::put(blocked);
	unlock();
}
